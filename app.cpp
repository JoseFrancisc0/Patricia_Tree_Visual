#include "app.h"

void App::initUI(){

    // Initialize header
    headerCont.setSize(sf::Vector2f(1300, 80));
    headerCont.setFillColor(sf::Color(0, 10, 85));
    headerCont.setOutlineColor(sf::Color::Black);
    headerCont.setOutlineThickness(2);
    headerCont.setPosition(0, 0);

    titleCont.setSize(sf::Vector2f(550, 40));
    titleCont.setFillColor(sf::Color::White);
    titleCont.setOutlineColor(sf::Color::Black);
    titleCont.setOutlineThickness(2);
    titleCont.setPosition(200, 20);

    title.setFont(font);
    title.setCharacterSize(30);
    title.setFillColor(sf::Color::Black);
    title.setString("Patricia Tree Visualization");
    title.setPosition(300, 22);

    // Initialize foot
    footCont.setSize(sf::Vector2f(1300, 80));
    footCont.setFillColor(sf::Color(0, 10, 85));
    footCont.setOutlineColor(sf::Color::Black);
    footCont.setOutlineThickness(2);
    footCont.setPosition(0, 640);

    inputField.setSize(sf::Vector2f(500, 60));
    inputField.setFillColor(sf::Color::White);
    inputField.setOutlineColor(sf::Color::Black);
    inputField.setOutlineThickness(2);
    inputField.setPosition(30, 650);

    inputText.setFont(font);
    inputText.setCharacterSize(30);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(40, 650);

    // Initialize button labels
    insertLabel.setFont(font);
    insertLabel.setCharacterSize(32);
    insertLabel.setFillColor(sf::Color::Black);
    insertLabel.setString("Insert");
    insertLabel.setPosition(575, 660);

    removeLabel.setFont(font);
    removeLabel.setCharacterSize(32);
    removeLabel.setFillColor(sf::Color::Black);
    removeLabel.setString("Remove");
    removeLabel.setPosition(770, 660);

    searchLabel.setFont(font);
    searchLabel.setCharacterSize(32);
    searchLabel.setFillColor(sf::Color::Black);
    searchLabel.setString("Search");
    searchLabel.setPosition(975, 660);

    // Initialize buttons
    insertButton.setSize(sf::Vector2f(150, 60));
    insertButton.setFillColor(sf::Color::Green);
    insertButton.setOutlineColor(sf::Color::Black);
    insertButton.setOutlineThickness(2);
    insertButton.setPosition(550, 650);

    removeButton.setSize(sf::Vector2f(150, 60));
    removeButton.setFillColor(sf::Color::Red);
    removeButton.setOutlineColor(sf::Color::Black);
    removeButton.setOutlineThickness(2);
    removeButton.setPosition(750, 650);

    searchButton.setSize(sf::Vector2f(150, 60));
    searchButton.setFillColor(sf::Color::Blue);
    searchButton.setOutlineColor(sf::Color::Black);
    searchButton.setOutlineThickness(2);
    searchButton.setPosition(950, 650);
}

void App::handleEvents(){
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        
        // Handle text input
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128) {
                if (std::isalnum(event.text.unicode) || event.text.unicode == ' ') {
                    inputText.setString(inputText.getString() + static_cast<char>(event.text.unicode));
                }
            }
        }

        // Handle backspacing
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Backspace) {
                // Get the current string in the input field
                std::string currentString = inputText.getString();

                // Check if the string is not empty
                if (!currentString.empty()) {
                    // Remove the last character from the string
                    currentString.pop_back();

                    // Update the input field text
                    inputText.setString(currentString);
                }
            }
        }

        // Handle button clicks
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (insertButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    // Handle insertion
                    searchPath.clear();
                    std::string word = inputText.getString(); // Get word from input field
                    tree->insert(word);
                }
                else if (removeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    // Handle removal
                    searchPath.clear();
                    std::string word = inputText.getString(); // Get word from input field
                    tree->remove(word);
                }
                else if (searchButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    // Handle search
                    searchPath.clear();
                    std::string word = inputText.getString(); // Get word from input field
                    bool found = tree->visualSearch(word, searchPath);

                    render();
                }
            }
        }
    }
}

void App::render(){
    window.clear(sf::Color(255, 223, 186));

    // Draw header
    window.draw(headerCont);
    window.draw(titleCont);
    window.draw(title);
    window.draw(inputText);

    // Draw foot
    window.draw(footCont);
    window.draw(inputField);
    window.draw(inputText);

    // Draw buttons
    window.draw(insertButton);
    window.draw(removeButton);
    window.draw(searchButton);
    window.draw(insertLabel);
    window.draw(removeLabel);
    window.draw(searchLabel);

    if(tree->getRoot() != nullptr)
        drawTree(tree->getRoot(), window, window.getSize().x / 2, 120, window.getSize().x / 4, 120, searchPath);

    window.display();
}

void App::drawTree(Node* current, sf::RenderWindow& window, float x, float y, float offsetX, float offsetY, vector<Node*>& searchPath){
    if (current == nullptr)
        return;

    float nodeSize = 30;

    bool highlight = std::find(searchPath.begin(), searchPath.end(), current) != searchPath.end();

    // Draw current node
    sf::CircleShape nodeShape(nodeSize);
    nodeShape.setOutlineThickness(3);

    if(highlight){
        nodeShape.setFillColor(sf::Color::White);
        nodeShape.setOutlineColor(sf::Color::Red);
    }
    else{
        nodeShape.setFillColor(sf::Color::White);
        nodeShape.setOutlineColor(sf::Color::Black);
    }
    nodeShape.setPosition(x - nodeSize, y - nodeSize);
    window.draw(nodeShape);

    // Draw current node's children
    float childY = y + offsetY;
    float childX = x - (offsetX * (current->children.size() - 1) / 2);

    for (auto& pair : current->children) {
        // Draw edge
        sf::Vertex edge[] = {
            sf::Vertex(sf::Vector2f(x, y), sf::Color::White),
            sf::Vertex(sf::Vector2f(childX, childY), sf::Color::White)
        };
        window.draw(edge, 2, sf::Lines);
        
        // Draw word associated with the edge
        sf::Text edgeText;
        edgeText.setFont(font);
        edgeText.setCharacterSize(20);
        edgeText.setFillColor(sf::Color::Black);
        edgeText.setString(pair.first); // Set the word associated with the edge
        // Adjust position to center the text along the edge
        edgeText.setPosition((x + childX) / 2 - edgeText.getLocalBounds().width / 2, (y + childY) / 2 - edgeText.getLocalBounds().height / 2);
        window.draw(edgeText);

        // Draw child node
        drawTree(pair.second, window, childX, childY, offsetX / 2, offsetY, searchPath);
        
        // Update child node position for next iteration
        childX += offsetX;
    }
}

void App::setPath(vector<Node*>& path){
    searchPath = path;
}

void App::run(){ 
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}