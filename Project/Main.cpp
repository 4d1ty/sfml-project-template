// An simple SFML Example
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ImGui/imgui.h>
#include <ImGui-SFML/imgui-SFML.h>

void propertyEditor(sf::RectangleShape&, sf::Color&);

int main() {
	unsigned int width = 800, height = 600;
	std::string title = "SFML Project Template";

	sf::RenderWindow window(sf::VideoMode(sf::Vector2u{ width, height }), title);
	sf::Color background(47, 50, 67);

	sf::RectangleShape player({ 100.f, 100.f });
	player.setPosition(sf::Vector2f{ static_cast<float>(width / 2), static_cast<float>(height / 2) });
	sf::Vector2f velocity(0.f, 0.f);
	float speed = 200.f;


	sf::Clock clock;

	if (!ImGui::SFML::Init(window)) {
		std::cout << "ImGui Cannot be initialized!\n";
	}

	while (window.isOpen()) {
		velocity.x = 0;
		velocity.y = 0;
		while (const auto& event = window.pollEvent()) {
			ImGui::SFML::ProcessEvent(window, *event);
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			velocity.x -= speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			velocity.x = speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			velocity.y -= speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			velocity.y += speed;
		}

		sf::Time dt = clock.restart();
		ImGui::SFML::Update(window, dt);
		float deltaTime = dt.asSeconds();
		int fps = static_cast<int>(1.f / deltaTime);

		player.move(velocity * deltaTime);

		propertyEditor(player, background);

		window.setTitle(title + " FPS: " + std::to_string(fps));
		window.clear(background);
		window.draw(player);
		ImGui::SFML::Render(window);
		window.display();
	}
	ImGui::SFML::Shutdown();
	std::cout << "Goodbye...\n";
	return 0;
}

void propertyEditor(sf::RectangleShape& player, sf::Color& backgroundColor) {
	ImGui::Begin("Property Editor");
	ImGui::Text("Player Properties");
	// Player Properties
	sf::Color playerColor = player.getFillColor();
	sf::Vector2f playerPosition = player.getPosition();
	sf::Vector2f playerSize = player.getSize();

	float pos[2] = { playerPosition.x, playerPosition.y };
	if (ImGui::DragFloat2("Position", pos, 1.f)) {
		player.setPosition({ pos[0], pos[1] });
		playerPosition = player.getPosition();
	}

	float sizeArr[2] = { playerSize.x, playerSize.y };
	if (ImGui::DragFloat2("Size", sizeArr, 1.0f, 10.f, 500.f)) {
		player.setSize(sf::Vector2f(sizeArr[0], sizeArr[1]));
		playerSize = player.getSize();
	}

	float pcol[4] = {
	playerColor.r / 255.f,
	playerColor.g / 255.f,
	playerColor.b / 255.f,
	playerColor.a / 255.f
	};
	if (ImGui::ColorEdit4("Player Color", pcol)) {
		playerColor = sf::Color(
			static_cast<uint8_t>(pcol[0] * 255),
			static_cast<uint8_t>(pcol[1] * 255),
			static_cast<uint8_t>(pcol[2] * 255),
			static_cast<uint8_t>(pcol[3] * 255)
		);
		player.setFillColor(playerColor);
	}
	ImGui::Text("World Properties");
	float wcol[4] = {
	backgroundColor.r / 255.f,
	backgroundColor.g / 255.f,
	backgroundColor.b / 255.f,
	backgroundColor.a / 255.f
	};
	if (ImGui::ColorEdit4("Background Color", wcol)) {
		backgroundColor = sf::Color(
			static_cast<uint8_t>(wcol[0] * 255),
			static_cast<uint8_t>(wcol[1] * 255),
			static_cast<uint8_t>(wcol[2] * 255),
			static_cast<uint8_t>(wcol[3] * 255)
		);
	}
	ImGui::End();
}