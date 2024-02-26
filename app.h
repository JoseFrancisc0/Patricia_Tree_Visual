#ifndef APP_H
#define APP_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "patricia_tree.h"

class App {
    private:
        sf::RenderWindow window;
        sf::Font font;

        // For header
        sf::Text title;
        sf::RectangleShape titleCont;
        sf::RectangleShape headerCont;

        // For footnote
        sf::RectangleShape footCont;

        sf::Text inputText;
        sf::RectangleShape inputField;

        sf::Text insertLabel;
        sf::Text removeLabel;
        sf::Text searchLabel;

        sf::RectangleShape insertButton;
        sf::RectangleShape removeButton;
        sf::RectangleShape searchButton;

        PatriciaTree* tree;
        vector<Node*> searchPath;

        void initUI();
        void handleEvents();
        void render();

        void drawTree(Node* current, sf::RenderWindow& window, float x, float y, float offsetX, float offsetY, vector<Node*>& searchPath);

    public:
        App() : window(sf::VideoMode(1280, 720), "Patricia Tree Visualization") {
            window.setKeyRepeatEnabled(false); // Disable key repeat events for text input

            if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
                std::cerr << "Failed to load font!" << std::endl;
                std::exit(1);
            }

            // Set background color
            window.clear(sf::Color(255, 223, 186)); // Light peach / coral background color

            // Initialize User Interface
            initUI();

            // Initialize Patricia Tree
            tree = new PatriciaTree(); // Assuming you have a class for Patricia Tree implementation
        }

        void run();
        void setPath(vector<Node*>& path);

        ~App(){
            delete tree;
        }
};

#endif // APP_H