#include "UIHelper.h"

// draws simple text at given position
void drawText(sf::RenderWindow& window,
    const sf::Font& font,
    const char* text,
    float x, float y,
    unsigned int size,
    sf::Color color) {

    sf::Text label(font, text, size);
    label.setFillColor(color);
    label.setPosition(sf::Vector2f(x, y));
    window.draw(label);
}

// draws a button rectangle with centered label
// returns true if mouse is hovering over it
bool drawButton(sf::RenderWindow& window,
    const sf::Font& font,
    const char* label,
    float x, float y,
    float width, float height,
    sf::Color btnColor,
    sf::Color textColor) {

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float mx = (float)mousePos.x;
    float my = (float)mousePos.y;

    bool hovering = isClicked(mx, my, x, y, width, height);

    sf::RectangleShape btn(sf::Vector2f(width, height));
    btn.setPosition(sf::Vector2f(x, y));

    if (hovering) {
        // slightly lighter when hovered
        btn.setFillColor(sf::Color(
            btnColor.r + 30 > 255 ? 255 : btnColor.r + 30,
            btnColor.g + 30 > 255 ? 255 : btnColor.g + 30,
            btnColor.b + 30 > 255 ? 255 : btnColor.b + 30
        ));
    }
    else {
        btn.setFillColor(btnColor);
    }

    btn.setOutlineThickness(1);
    btn.setOutlineColor(sf::Color::White);
    window.draw(btn);

    sf::Text text(font, label, 18);
    text.setFillColor(textColor);

    // center text in button
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition(sf::Vector2f(
        x + (width - bounds.size.x) / 2.0f,
        y + (height - bounds.size.y) / 2.0f - 4
    ));

    window.draw(text);

    return hovering;
}

// draws an input text box
void drawInputBox(sf::RenderWindow& window,
    const sf::Font& font,
    const char* content,
    float x, float y,
    float width, float height,
    bool isActive) {

    sf::RectangleShape box(sf::Vector2f(width, height));
    box.setPosition(sf::Vector2f(x, y));
    box.setFillColor(sf::Color(30, 30, 30));

    if (isActive) {
        box.setOutlineColor(sf::Color::Cyan);
    }
    else {
        box.setOutlineColor(sf::Color(100, 100, 100));
    }

    box.setOutlineThickness(2);
    window.draw(box);

    sf::Text text(font, content, 18);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(x + 8, y + 8));
    window.draw(text);
}

// checks if a point is inside a rectangle area
bool isClicked(float mx, float my,
    float x, float y,
    float width, float height) {

    return mx >= x && mx <= x + width &&
        my >= y && my <= y + height;
}

// draws a simple message box with text inside
void drawMessageBox(sf::RenderWindow& window,
    const sf::Font& font,
    const char* message,
    float x, float y,
    float width, float height) {

    sf::RectangleShape box(sf::Vector2f(width, height));
    box.setPosition(sf::Vector2f(x, y));
    box.setFillColor(sf::Color(20, 20, 20));
    box.setOutlineColor(sf::Color(80, 80, 80));
    box.setOutlineThickness(1);
    window.draw(box);

    sf::Text text(font, message, 16);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(x + 10, y + 10));
    window.draw(text);
}