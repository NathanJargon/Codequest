#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::string wrapText(std::string text, unsigned width, const sf::Font &font, unsigned characterSize, bool bold = false)
{
    unsigned currentLineLength = 0;
    std::string wordsBuffer;
    std::string wrappedText;
    std::istringstream words(text);
    std::string word;

    while (words >> word)
    {
        sf::Text line(wordsBuffer + " " + word, font, characterSize);
        currentLineLength = line.getLocalBounds().width;

        if (currentLineLength > width)
        {
            wrappedText += wordsBuffer + "\n";
            wordsBuffer = word;
        }
        else
        {
            wordsBuffer += " " + word;
        }
    }

    return wrappedText + wordsBuffer;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "CodeQuest");

    sf::Font font;
    if (!font.loadFromFile("fonts/Montserrat Light.otf")) {
        std::cerr << "Could not load font" << std::endl;
    }

    sf::Text title("CodeQuest", font, 30);
    title.setPosition(window.getSize().x - title.getLocalBounds().width - 25, 25);
    title.setStyle(sf::Text::Bold);

    sf::RectangleShape sidebar(sf::Vector2f(200, window.getSize().y));
    sidebar.setFillColor(sf::Color(50, 50, 50));

    std::vector<sf::RectangleShape> buttons;
    for (int i = 0; i < 5; i++) {
        sf::RectangleShape button(sf::Vector2f(180, 70));
        button.setPosition(10, 10 + i * 80); 
        button.setFillColor(sf::Color::White); 
        buttons.push_back(button);
    }

    std::vector<std::string> buttonNames = {
        "Elementary Programming", 
        "Functions", 
        "Loops",
        "Arrays",
        "Object and Classes",
        "Object Oriented Programming",
        "Pointers and Dynamic Memory Management",
        "Templates, Vectors, and Stacks",
        "File Input and Output",
        "Operator Overloading",
        "Exception Handling",
        "Recursion",
        };

    sf::Text sidebarTitle("Lessons", font, 30);
    sidebarTitle.setPosition(10, 10);
    sidebarTitle.setFillColor(sf::Color::White);

    sf::Text nextButtonText("Next", font, 25);
    sf::Text prevButtonText("Previous", font, 25);

    sf::RectangleShape nextButton(sf::Vector2f(180, 70));
    nextButton.setPosition(10, 10 + 5 * 80);
    nextButton.setFillColor(sf::Color::Green);
    sf::RectangleShape prevButton(sf::Vector2f(180, 70));
    prevButton.setPosition(10, 10 + 6 * 80);
    prevButton.setFillColor(sf::Color::Red);


    std::vector<sf::Text> buttonLabels;
    for (int i = 0; i < 12; i++) {
        std::string wrappedText = wrapText(buttonNames[i], 90, font, 8);
        sf::Text label(wrappedText, font, 15);
        float x = buttons[i].getPosition().x + buttons[i].getSize().x / 2 - label.getLocalBounds().width / 2;
        float y = buttons[i].getPosition().y + buttons[i].getSize().y / 2 - label.getLocalBounds().height / 2;
        label.setPosition(x, y);
        label.setFillColor(sf::Color::Black); 
        buttonLabels.push_back(label);
    }

    std::vector<sf::Text> texts;
    for (int i = 0; i < 12; i++) {
        std::ifstream file("texts/text" + std::to_string(i) + ".txt");
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        sf::Text text(str, font, 30);
        text.setPosition(window.getSize().x / 2, window.getSize().y / 2);
        texts.push_back(text);
    }

    sf::Text currentText = texts[0];
    int currentPage = 0;
    int totalPages = (buttonNames.size() + 4) / 5; // Round up
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close(); 
    
            if (event.type == sf::Event::MouseButtonPressed) {
                for (int i = 0; i < buttons.size(); i++) {
                    if (buttons[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                        currentText = texts[i + currentPage * 5];
                    }
                }
    
                // Update the current page when the navigation buttons are clicked
                // and prevent going to a page that doesn't exist
                if (nextButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    if (currentPage < totalPages - 1) {
                        currentPage++;
                    }
                }
                if (prevButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    if (currentPage > 0) {
                        currentPage--;
                    }
                }
            }
        }
    
        nextButtonText.setPosition(
            nextButton.getPosition().x + nextButton.getSize().x / 2 - nextButtonText.getLocalBounds().width / 2,
            nextButton.getPosition().y + nextButton.getSize().y / 2 - nextButtonText.getLocalBounds().height / 2
        );
        prevButtonText.setPosition(
            prevButton.getPosition().x + prevButton.getSize().x / 2 - prevButtonText.getLocalBounds().width / 2,
            prevButton.getPosition().y + prevButton.getSize().y / 2 - prevButtonText.getLocalBounds().height / 2
        );

        window.clear();
        window.draw(sidebar);
        window.draw(sidebarTitle);
    
        // Only draw the buttons and labels for the current page
        // and prevent drawing buttons or labels that don't exist
        for (int i = 0; i < buttons.size() && i + currentPage * 5 < buttonNames.size(); i++) {
            window.draw(buttons[i]);
    
            // Update the position of the labels
            float x = buttons[i].getPosition().x + buttons[i].getSize().x / 2 - buttonLabels[i + currentPage * 5].getLocalBounds().width / 2;
            float y = buttons[i].getPosition().y + buttons[i].getSize().y / 2 - buttonLabels[i + currentPage * 5].getLocalBounds().height / 2;
            buttonLabels[i + currentPage * 5].setPosition(x, y);
    
            window.draw(buttonLabels[i + currentPage * 5]);
        }
    
        window.draw(nextButton);
        window.draw(prevButton);

        window.draw(nextButtonText);
        window.draw(prevButtonText);

        window.draw(currentText);
        window.draw(title);
        window.display();
    }
    

    return 0;
}