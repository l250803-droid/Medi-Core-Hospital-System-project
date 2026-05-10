#pragma once
#include <SFML/Graphics.hpp>

// draws a simple text label on the window
void drawText(sf::RenderWindow& window,
    const sf::Font& font,
    const char* text,
    float x, float y,
    unsigned int size,
    sf::Color color);

// draws a rectangle button and returns true if mouse is hovering
bool drawButton(sf::RenderWindow& window,
    const sf::Font& font,
    const char* label,
    float x, float y,
    float width, float height,
    sf::Color btnColor,
    sf::Color textColor);

// draws an input box and returns the current text inside it
void drawInputBox(sf::RenderWindow& window,
    const sf::Font& font,
    const char* content,
    float x, float y,
    float width, float height,
    bool isActive);

// checks if mouse click is inside a rectangle
bool isClicked(float mx, float my,
    float x, float y,
    float width, float height);

// draws a scrollable message box
void drawMessageBox(sf::RenderWindow& window,
    const sf::Font& font,
    const char* message,
    float x, float y,
    float width, float height);
