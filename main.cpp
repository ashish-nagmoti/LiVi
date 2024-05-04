#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Node
{
public:
    sf::RectangleShape shape;
    sf::RectangleShape divider;
    sf::Text text;
    sf::Text endText;
    Node *next;
    sf::Font font; // Add the missing font member variable

    Node(float width = 60.f, float height = 30.f) : shape(sf::Vector2f(width, height)), divider(sf::Vector2f(2.f, height))
    {
        shape.setFillColor(sf::Color::Blue);
        divider.setFillColor(sf::Color::White);
        next = nullptr;

        font.loadFromFile("./src/t.ttf");     // Initialize the font
        endText.setFont(font);                // Set the font
        endText.setString("X");               // Set the string to 'X'
        endText.setCharacterSize(30);         // Set the character size
        endText.setFillColor(sf::Color::Red); // Set the fill color
    }
};

class LinkedList
{
public:
    Node *head;
    std::vector<Node *> nodes;
    sf::Font font;
    bool searching;
    std::string searchValue;
    int searchIndex;
    sf::Clock clock;
    float delay = 1.0f;
    bool found = false;
    int foundPosition = -1; // Add a counter to keep track of the position where the element is found
    sf::Clock foundClock;
    sf::Clock nfoundClock;
    int dis = 1;
    LinkedList() : head(nullptr), searching(false), searchIndex(0)
    {
        font.loadFromFile("./src/t.ttf");
    }

    void addNode(const std::string &value)
    {
        Node *node = new Node();
        node->text.setFont(font);
        node->text.setString(value);
        node->text.setCharacterSize(30);
        node->text.setFillColor(sf::Color::White);

        if (head == nullptr)
        {
            head = node;
        }
        else
        {
            // Find the last node
            Node *current = head;
            while (current->next != nullptr)
            {
                current = current->next;
            }

            // Add the new node at the end of the list
            current->next = node;
        }

        nodes.clear();
        Node *current = head;
        while (current != nullptr)
        {
            nodes.push_back(current);
            current = current->next;
        }
    }
    void deleteNode(const std::string &value)
    {
        Node *current = head;
        Node *previous = nullptr;

        while (current != nullptr)
        {
            if (current->text.getString() == value)
            {
                if (previous == nullptr)
                {
                    head = current->next;
                }
                else
                {
                    previous->next = current->next;
                }
                nodes.erase(std::remove(nodes.begin(), nodes.end(), current), nodes.end());
                delete current;
                return;
            }
            previous = current;
            current = current->next;
        }
    }
    void search(const std::string &value)
    {
        searching = true;
        dis = 1;
        searchValue = value;
        searchIndex = 0;
        found = false;
    }

    void draw(sf::RenderWindow &window)
    {
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            nodes[i]->shape.setPosition(100.f + i * 100.f, 200.f);
            nodes[i]->divider.setPosition(nodes[i]->shape.getPosition().x + nodes[i]->shape.getSize().x / 2, nodes[i]->shape.getPosition().y);
            sf::FloatRect textRect = nodes[i]->text.getLocalBounds();
            nodes[i]->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            nodes[i]->text.setPosition(sf::Vector2f(nodes[i]->shape.getPosition().x + nodes[i]->shape.getSize().x / 4, nodes[i]->shape.getPosition().y + nodes[i]->shape.getSize().y / 2));
            window.draw(nodes[i]->shape);
            window.draw(nodes[i]->divider);
            if (i == nodes.size() - 1)
            {
                sf::FloatRect dividerRect = nodes[i]->divider.getGlobalBounds();
            sf::FloatRect endTextRect = nodes[i]->endText.getLocalBounds();

            nodes[i]->endText.setOrigin(endTextRect.left + endTextRect.width / 223.5f, endTextRect.top + endTextRect.height / 2.0f);
            nodes[i]->endText.setPosition(dividerRect.left + dividerRect.width / 228.0f, dividerRect.top + dividerRect.height / 2.0f);

            window.draw(nodes[i]->endText);
            }
            window.draw(nodes[i]->text);

            // Draw an arrow
            if (i < nodes.size() - 1)
            {
                sf::VertexArray line(sf::Lines, 2);
                line[0].position = nodes[i]->shape.getPosition() + sf::Vector2f(nodes[i]->shape.getSize().x, nodes[i]->shape.getSize().y / 2);
                line[1].position = nodes[i + 1]->shape.getPosition() + sf::Vector2f(0, nodes[i + 1]->shape.getSize().y / 2);
                window.draw(line);
            }

            // Draw an arrow and a label
            if (i == 0)
            {
                sf::VertexArray arrow(sf::Lines, 2);
                arrow[0].position = nodes[i]->shape.getPosition() + sf::Vector2f(-10.f, 0.f);
                arrow[1].position = nodes[i]->shape.getPosition();
                window.draw(arrow);

                sf::Text label("Head", font, 20);
                label.setFillColor(sf::Color::Red);
                label.setPosition(arrow[0].position + sf::Vector2f(-40.f, -30.f));
                window.draw(label);
            }

            // Draw a special marker for the node that's currently being searched
            if (!searching)
            {
                nodes[i]->shape.setFillColor(sf::Color::Blue);
            }
            if (searching && i == searchIndex)
            {
                if (clock.getElapsedTime().asSeconds() > delay)
                {
                    if (nodes[i]->text.getString() == searchValue)
                    {
                        nodes[i]->shape.setFillColor(sf::Color::Green);
                        std::cout << "Element found" << std::endl;
                        searching = false;
                        found = true;
                        foundPosition = i + 1;
                        foundClock.restart();
                    }
                    else
                    {
                        nodes[i]->shape.setFillColor(sf::Color::Red);
                        searchIndex++;

                        if (searchIndex >= nodes.size())
                        {
                            dis = 0;
                            searching = false;
                            if (!found)
                            {
                                nfoundClock.restart();
                            }
                        }
                    }
                    clock.restart();
                }
            }
        }
        if (found && foundClock.getElapsedTime().asSeconds() <= 3.0f)
        {
            sf::RectangleShape rectangle(sf::Vector2f(400.f, 100.f));
            rectangle.setFillColor(sf::Color::White);
            rectangle.setPosition(window.getSize().x / 2 - rectangle.getSize().x / 2, window.getSize().y / 2 - rectangle.getSize().y / 2);
            window.draw(rectangle);

            sf::Text text("Element found at position " + std::to_string(foundPosition) + "!", font, 20);
            text.setFillColor(sf::Color::Black);
            text.setPosition(rectangle.getPosition().x + rectangle.getSize().x / 2 - text.getLocalBounds().width / 2, rectangle.getPosition().y + rectangle.getSize().y / 2 - text.getLocalBounds().height / 2);
            window.draw(text);
        }
        else if (dis == 0 && nfoundClock.getElapsedTime().asSeconds() <= 3.0f && !found)
        {
            // Draw a rectangle
            sf::RectangleShape rectangle(sf::Vector2f(200.f, 100.f));
            rectangle.setFillColor(sf::Color::White);
            rectangle.setPosition(window.getSize().x / 2 - rectangle.getSize().x / 2, window.getSize().y / 2 - rectangle.getSize().y / 2);
            window.draw(rectangle);

            // Draw some text
            sf::Text text("Element not found!", font, 20);
            text.setFillColor(sf::Color::Black);
            text.setPosition(rectangle.getPosition().x + rectangle.getSize().x / 2 - text.getLocalBounds().width / 2, rectangle.getPosition().y + rectangle.getSize().y / 2 - text.getLocalBounds().height / 2);
            window.draw(text);
        }
    }
};
int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 400), "LiVi");

    // window.setFramerateLimit(2);
    LinkedList list;
    std::string input;

    sf::Text inputText;
    sf::Font font;
    font.loadFromFile("./src/t.ttf");
    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(10.f, 320.f);

    sf::Text addText("Add Element (Press A)", font, 20);
    addText.setFillColor(sf::Color::White);
    addText.setPosition(10.f, 360.f);

    sf::Text searchText("Search Element (Press S)", font, 20);
    searchText.setFillColor(sf::Color::White);
    searchText.setPosition(280.f, 360.f);

    sf::Text deleteText("Delete Element (Press D)", font, 20);
    deleteText.setFillColor(sf::Color::White);
    deleteText.setPosition(580.f, 360.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b' && !input.empty())
                {
                    input.pop_back();
                }
                else if (event.text.unicode < 128)
                {
                    input += static_cast<char>(event.text.unicode);
                }
                inputText.setString(input);
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::A)
                {
                    list.addNode(input);
                    input.clear();
                }
                if (event.key.code == sf::Keyboard::S)
                {

                    list.search(input);
                    input.clear();
                    window.display();
                }
                if (event.key.code == sf::Keyboard::D)
                {

                    list.deleteNode(input);
                    input.clear();
                }
            }
        }

        window.clear();
        list.draw(window);
        window.draw(inputText);
        window.draw(addText);
        window.draw(searchText);
        window.draw(deleteText);
        window.display();
    }

    return 0;
}