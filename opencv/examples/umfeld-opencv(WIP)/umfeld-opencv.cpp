#include "Umfeld.h"
#include "UmfeldOpenCV.h"

using namespace umfeld;

class UmfeldApp : public PApplet {

    PFont*                 mFont{};

    void settings() override {
        size(1024, 768);
    }

    void setup() override {
        mFont = loadFont("../RobotoMono-Regular.ttf", 48);
        textFont(mFont);
        foobar();
    }

    void draw() override {
        background(1);
        fill(0);
        noStroke();
        text("23", mouseX, mouseY);
    }

    void mousePressed() override {
        retrieve_contour();
    }

    void mouseReleased() override {
    }
};

PApplet* umfeld::instance() {
    return new UmfeldApp();
}