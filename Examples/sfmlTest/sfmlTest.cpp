#include <SFML/Graphics.hpp>
#include <vector>

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Open Canvas");

    // Vector to store lines (each line is a pair of points)
    std::vector<sf::VertexArray> lines;

    // Variables to store the start and end points of a line
    sf::Vector2f startPoint, endPoint;
    bool drawing = false;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Handle window close event
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle mouse button press
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!drawing) {
                        // Start drawing a new line
                        startPoint = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                        drawing = true;
                    } else {
                        // Finish drawing the line
                        endPoint = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                        drawing = false;

                        // Create a line (sf::VertexArray) and add it to the vector
                        sf::VertexArray line(sf::Lines, 2);
                        line[0].position = startPoint;
                        line[1].position = endPoint;
                        line[0].color = sf::Color::White;
                        line[1].color = sf::Color::White;
                        lines.push_back(line);
                    }
                }
            }
        }

        // Clear the window with black color
        window.clear(sf::Color::Black);

        // Draw all lines
        for (const auto& line : lines) {
            window.draw(line);
        }

        // Display the window
        window.display();
    }

    return 0;
}
