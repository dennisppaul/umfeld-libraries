// TODO this is more like a proof of concept ;)

#include "Umgebung.h"
#include "UmgebungOllama.h"

using namespace umgebung;

UmgebungOllama ollama;
std::string    querry = "Hello, how are you?";
std::string    response;

void settings() {
    size(1024, 768);
}

void setup() {
    background(0.85f);
    noStroke();
    fill(0);

    ollama.set_URL("http://localhost:11434");

    const std::vector<std::string> models = ollama.get_installed_models();
    console("Installed Model: ");
    for (const auto& model: models) {
        console(model);
    }

    ollama.set_model("llama3.2:latest");
    response = ollama.generate(querry);
}

void draw() {
    background(0.85f);
    debug_text(querry, 30, 35);
    int y = 60;
    for (size_t i = 0; i < response.length(); i += 30) {
        std::string line = response.substr(i, 30);
        debug_text(line, 30, y);
        y += 15;
    }
}
