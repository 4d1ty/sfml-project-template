// An simple SFML Example
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ImGui/imgui.h>
#include <ImGui-SFML/imgui-SFML.h>

void propertyEditor(sf::RectangleShape&, sf::Color&, float&, float&, float&, float&);

int main() {
	unsigned int width = 800, height = 800;
	std::string title = "SFML Project Template";

	sf::RenderWindow window(sf::VideoMode(sf::Vector2u{ width, height }), title, sf::Style::Close);
	sf::Color background(61, 99, 187);

	sf::RectangleShape player({ 100.f, 100.f });
	player.setPosition(sf::Vector2f{ static_cast<float>(width / 2), static_cast<float>(height / 2) });
	float speed = 1000.f;
	float jumpSpeed = 2500.f;
	bool isGrounded = false;
	float repeatDelta = 10.f;
	float groundDelta = 10.f;

	sf::Vector2f velocity(0.f, 0.f);
	sf::Vector2f gravity(0.f, 10000.f);
	float groundThickness{ 100.f };
	sf::RectangleShape ground({ static_cast<float>(width), groundThickness + groundDelta });
	ground.setFillColor(sf::Color(86, 205, 100));



	sf::Clock clock;

	if (!ImGui::SFML::Init(window)) {
		std::cout << "ImGui Cannot be initialized!\n";
	}

	ImGui::StyleColorsClassic();

	while (window.isOpen()) {
		velocity.x = 0.f;
		while (const auto& event = window.pollEvent()) {
			ImGui::SFML::ProcessEvent(window, *event);
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		sf::Time dt = clock.restart();
		ImGui::SFML::Update(window, dt);
		float deltaTime = dt.asSeconds();
		int fps = static_cast<int>(1.f / deltaTime);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			velocity.x -= speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			velocity.x = speed;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && isGrounded)
		{
			velocity.y -= jumpSpeed;
		}


		player.move(velocity * deltaTime);

		const auto& [playerXSize, playerYSize] = player.getSize();
		const auto& [playerXPosition, playerYPosition] = player.getPosition();

		float groundY = height - groundThickness - groundDelta;
		ground.setPosition(sf::Vector2f{ 0.f, groundY });


		velocity += gravity * deltaTime;

		if (playerXPosition > (width + playerXSize + repeatDelta)) {
			player.setPosition({ 0.f - playerXSize - repeatDelta, playerYPosition });
		}

		if (playerXPosition < (0.f - playerXSize - repeatDelta)) {
			player.setPosition({ width + playerXSize + repeatDelta, playerYPosition });
		}

		/*if (playerYPosition > (height + playerYSize + repeatDelta)) {
			player.setPosition({ playerXPosition, 0.f - playerYSize - repeatDelta });
		}

		if (playerYPosition < (0.f - playerYSize - repeatDelta)) {
			player.setPosition({ playerXPosition, height + playerYSize + repeatDelta });
		}*/

		if (playerYPosition + playerYSize >= groundY) {
			velocity.y = 0.f;
			isGrounded = true;
		}
		else {
			isGrounded = false;
		}


		propertyEditor(player, background, speed, repeatDelta, velocity.x, velocity.y);

		window.setTitle(title + " FPS: " + std::to_string(fps));
		window.clear(background);
		window.draw(ground);
		window.draw(player);
		ImGui::SFML::Render(window);
		window.display();
	}
	ImGui::SFML::Shutdown();
	std::cout << "Goodbye...\n";
	return 0;
}

void propertyEditor(
	sf::RectangleShape& player,
	sf::Color& backgroundColor,
	float& speed,
	float& repeatDelta,
	float& velocityX,
	float& velocityY) {
	ImGui::Begin("Property Editor");
	ImGui::Text("Player Properties");

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

	ImGui::Text("Velocity x:%f, y:%f", velocityX, velocityY);


	ImGui::DragFloat("Player Speed", &speed, 1.f, 10.f, 10000.f);
	ImGui::DragFloat("Repeat Delta", &repeatDelta, 1.f, 1.f, 10.f);



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