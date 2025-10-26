/*
 * Umfeld
 *
 * This file is part of the *Umfeld* library (https://github.com/dennisppaul/umfeld).
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

#pragma once

#include <string>

namespace umfeld {
    class UmfeldOllama {
    public:
        std::string              generate(const std::string& prompt);
        std::vector<std::string> get_installed_models() const;

        void set_model(const std::string& new_model) {
            model = new_model;
        }

        void set_URL(const std::string& new_URL) {
            URL = new_URL;
        }

        void generate_stream(const std::string& prompt, std::function<void(const std::string&)> on_chunk);

    private:
        struct StreamContext {
            std::function<void(const std::string&)> on_chunk;
            std::string                             buffer;
        };

        std::string       model                = "llama3.2:latest";
        std::string       URL                  = "http://localhost:11434";
        const std::string CMD_GENERATE         = "/api/generate";
        const std::string CMD_INSTALLED_MODELS = "/api/tags";

        std::string extract_response_text(const std::string& raw_response);

        static size_t StreamWriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata);

        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
            const size_t totalSize = size * nmemb;
            output->append(static_cast<char*>(contents), totalSize);
            return totalSize;
        }
    };
} // namespace umfeld
