#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Node
{
public:
    sf::CircleShape shape;
    sf::Text text;
    Node *next;

    Node(float radius = 20.f) : shape(radius)
    {
        shape.setFillColor(sf::Color::Blue);
        next = nullptr;
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

        if (head == nullptr || std::stoi(head->text.getString().toAnsiString()) >= std::stoi(value))
        {

            node->next = head;
            head = node;
        }
        else
        {
            // Find the first node with a greater value
            Node *current = head;
            while (current->next != nullptr && std::stoi(current->next->text.getString().toAnsiString()) < std::stoi(value))
            {
                current = current->next;
            }

            // Insert the new node after the found node
            node->next = current->next;
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
            nodes[i]->shape.setPosition(100.f + i * 60.f, 200.f);
            sf::FloatRect textRect = nodes[i]->text.getLocalBounds();
            nodes[i]->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            nodes[i]->text.setPosition(sf::Vector2f(nodes[i]->shape.getPosition().x + nodes[i]->shape.getRadius(), nodes[i]->shape.getPosition().y + nodes[i]->shape.getRadius()));
            window.draw(nodes[i]->shape);
            window.draw(nodes[i]->text);

            // Draw an arrow 
            if (i < nodes.size() - 1)
            {
                sf::VertexArray line(sf::Lines, 2);
                line[0].position = nodes[i]->shape.getPosition() + sf::Vector2f(nodes[i]->shape.getRadius(), nodes[i]->shape.getRadius());
                line[1].position = nodes[i + 1]->shape.getPosition() + sf::Vector2f(nodes[i + 1]->shape.getRadius(), nodes[i + 1]->shape.getRadius());
                window.draw(line);
            }

            // Draw an arrow and a label 
            if (i == 0)
            {
                sf::VertexArray arrow(sf::Lines, 2);
                arrow[0].position = nodes[i]->shape.getPosition() + sf::Vector2f(-10.f, 0.f);
                arrow[1].position = nodes[i]->shape.getPosition();
                window.draw(arrow);

                sf::Text label("Root", font, 20);
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
        if (found && foundClock.getElapsedTime().asSeconds() <= 10.0f)
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
        else if (dis == 0 && nfoundClock.getElapsedTime().asSeconds() <= 5.0f && !found)
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