/*
 * Umgebung
 *
 * This file is part of the *Umgebung* library (https://github.com/dennisppaul/umgebung).
 * Copyright (c) 2025 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <curl/curl.h>
#include <string>
#include <sstream>

#include <nlohmann/json.hpp>

#include "Umgebung.h"
#include "UmgebungOllama.h"

using namespace umgebung;

size_t UmgebungOllama::StreamWriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t totalSize = size * nmemb;
    auto*  context   = static_cast<StreamContext*>(userdata);

    context->buffer.append(ptr, totalSize);

    size_t pos = 0;
    while ((pos = context->buffer.find('\n')) != std::string::npos) {
        std::string line = context->buffer.substr(0, pos);
        context->buffer.erase(0, pos + 1);

        if (line.empty()) {
            continue;
        }

        try {
            auto json = nlohmann::json::parse(line);
            if (json.contains("response")) {
                context->on_chunk(json["response"].get<std::string>());
            }
        } catch (...) {
            // malformed json line, ignore
        }
    }

    return totalSize;
}

void UmgebungOllama::generate_stream(const std::string& prompt, std::function<void(const std::string&)> on_chunk) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return;
    }

    const std::string url         = URL + CMD_GENERATE;
    const std::string jsonPayload = R"({"model":")" + model + R"(","prompt":")" + prompt + R"("})";

    struct curl_slist* headers = nullptr;
    headers                    = curl_slist_append(headers, "Content-Type: application/json");

    StreamContext context{on_chunk};

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, StreamWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &context);

    curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}


std::vector<std::string> UmgebungOllama::get_installed_models() {
    std::vector<std::string> models;
    CURL*                    curl = curl_easy_init();
    if (!curl) {
        return models;
    }
    const std::string url = URL + CMD_INSTALLED_MODELS;
    std::string       response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        return models;
    }

    for (auto model: models) {
        console("Model: " + model);
    }

    try {
        auto json = nlohmann::json::parse(response);
        if (json.contains("models") && json["models"].is_array()) {
            for (const auto& model: json["models"]) {
                if (model.contains("name")) {
                    models.push_back(model["name"].get<std::string>());
                }
            }
        }
    } catch (...) {
        warning("Failed to parse JSON response");
        // handle parse error or bad response
    }

    return models;
}

std::string UmgebungOllama::extract_response_text(const std::string& raw_response) {
    std::stringstream ss(raw_response);
    std::string       line;
    std::string       result;

    while (std::getline(ss, line)) {
        try {
            if (line.empty()) {
                continue;
            }
            auto json = nlohmann::json::parse(line);
            if (json.contains("response")) {
                result += json["response"].get<std::string>();
            }
        } catch (const std::exception& e) {
            continue;
        }
    }

    return result;
}

std::string UmgebungOllama::generate(const std::string& prompt) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return "Curl init failed";
    }

    const std::string url         = URL + CMD_GENERATE;
    const std::string jsonPayload = R"({"model":")" + model + R"(","prompt":")" + prompt + R"("})";
    std::string       response;

    struct curl_slist* headers = nullptr;
    headers                    = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    const CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        response = "Curl request failed: " + std::string(curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return extract_response_text(response);
}
