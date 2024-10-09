#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

const int MAX_CIRCLES = 2000; // Максимальное количество окружностей
int ii = 0;

// Функция для создания промежуточных окружностей между двумя точками
std::vector<sf::RectangleShape> createPoints(sf::Vector2f v1, sf::Vector2f v2, float radius) {
    std::vector<sf::RectangleShape> points;

    // Вычисляем расстояние между точками
    float distance = std::sqrt((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));

    // Если расстояние между точками больше радиуса, создаем промежуточные окружности
    if (distance > radius / 2) {
        int count = static_cast<int>(distance / (radius / 2)); // Количество окружностей
        sf::Vector2f step((v2.x - v1.x) / count, (v2.y - v1.y) / count); // Шаг по линии между точками

        for (int i = 1; i < count; ++i) {
            //sf::CircleShape point(radius);
            //point.setOrigin(radius, radius); // Центрирование окружности
            ///point.setFillColor(sf::Color::Blue);
            sf::RectangleShape point(sf::Vector2f(8.f, 8.f));
            point.setOrigin(8.f, 8.f);  // Центрирование круга
            point.setFillColor(sf::Color::Red); // Цвет курсора
            point.setPosition(v1.x + step.x * i, v1.y + step.y * i); // Позиция промежуточной окружности
            points.push_back(point);
        }
    }

    return points;
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8; // Сглаживание
    sf::RenderWindow window(sf::VideoMode(1280, 720), "App", sf::Style::Default, settings);
    window.setFramerateLimit(160);

    // Шрифт для текста FPS
    sf::Font font;
    if (!font.loadFromFile("../../../Roboto-Medium.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    // Текст для отображения FPS
    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(24);
    fpsText.setFillColor(sf::Color::Black);
    fpsText.setPosition(10.f, 10.f); // Позиция текста в правом верхнем углу

    sf::Vector2f last_pos;  // Для хранения последней позиции
    bool is_pos_initialized = false;  // Флаг для проверки инициализации
    bool isDraw = false;  // Флаг для рисования

    std::vector<sf::RectangleShape> circles; // Для хранения всех окружностей
    //std::vector<sf::CircleShape> circles; // Для хранения всех окружностей
    //sf::CircleShape cursor(10.f); // Радиус курсора
    sf::RectangleShape cursor(sf::Vector2f(10.f, 10.f)); // Радиус курсора
    //cursor.setOrigin(cursor.getRadius(), cursor.getRadius());  // Центрирование круга
    cursor.setOrigin(8.f, 8.f);  // Центрирование круга
    cursor.setFillColor(sf::Color::Red); // Цвет курсора

    sf::RenderTexture renderTexture; // Для сохранения состояния экрана
    if (!renderTexture.create(1280, 720)) {
        std::cerr << "Error creating render texture!" << std::endl;
        return -1;
    }

    sf::Clock clock; // Для отслеживания времени
    int frameCount = 0; // Счетчик кадров
    float fps = 0; // Переменная для хранения FPS

    while (window.isOpen()) {
        sf::Event e;

        // Логика обработки событий
        while (window.pollEvent(e)) {
            switch (e.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::Resized:
                    std::cout << "new width: " << e.size.width << std::endl;
                    std::cout << "new height: " << e.size.height << std::endl;
                    break;

                case sf::Event::MouseButtonPressed:
                    if (e.mouseButton.button == sf::Mouse::Left) {
                        isDraw = true;
                        std::cout << "isDraw: active" << std::endl;
                    }
                    break;

                case sf::Event::KeyPressed: // Обработка нажатия клавиш
                    if (e.key.code == sf::Keyboard::C) { // Проверяем, нажата ли клавиша "C"
                        renderTexture.clear(sf::Color::White); // Очищаем текстуру
                        circles.clear(); // Очищаем вектор окружностей
                        std::cout << "Cleared circles and render texture." << std::endl;
                    }
                    break;

                case sf::Event::MouseButtonReleased:
                    if (e.mouseButton.button == sf::Mouse::Left) {
                        isDraw = false;
                        std::cout << "isDraw: disable" << std::endl;
                        is_pos_initialized = false;

                        // Проверка на превышение количества окружностей
                        if (circles.size() > MAX_CIRCLES) {
                            // Сохраняем текущее состояние на текстуру
                            renderTexture.clear(sf::Color::White);
                            for (const auto& circle : circles) {
                                renderTexture.draw(circle);
                            }
                            renderTexture.display();
                            
                            // Очищаем вектор окружностей
                            circles.clear();
                            std::cout << "Cleared circle array after exceeding max limit." << std::endl;
                        }
                    }
                    break;

                case sf::Event::MouseMoved:
                    if (isDraw) {
                        std::cout << (++ii % MAX_CIRCLES) << " ";
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f current_pos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                        cursor.setPosition(current_pos);

                        if (is_pos_initialized) {
                            // Добавляем промежуточные окружности между последней позицией и новой позицией курсора
                            //std::vector<sf::CircleShape> newCircles = createPoints(last_pos, current_pos, cursor.getRadius());
                            std::vector<sf::RectangleShape> newCircles = createPoints(last_pos, current_pos, 4.f);
                            circles.insert(circles.end(), newCircles.begin(), newCircles.end()); // Добавляем новые окружности
                        }

                        // Добавляем текущую позицию курсора как очередную окружность
                        circles.push_back(cursor);
                        last_pos = current_pos;
                        is_pos_initialized = true;  // Позиция установлена
                    }
                    break;

                default:
                    break;
            }
        }

        // Отрисовка
        window.clear(sf::Color::White);

        // Рисуем сохраненный холст, если он существует
        if (!circles.empty()) {
            sf::Sprite savedSprite(renderTexture.getTexture());
            window.draw(savedSprite); // Отображаем сохраненный холст
        }

        // Рисуем все окружности, которые были добавлены
        for (const auto& circle : circles) {
            window.draw(circle);
        }

        // Отображение FPS
        frameCount++;
        if (clock.getElapsedTime().asSeconds() >= 1.0f) {
            fps = static_cast<float>(frameCount);
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
            frameCount = 0;
            clock.restart();
        }
        window.draw(fpsText); // Рисуем текст FPS

        // Отображение
        window.display();
    }

    return 0;
}
