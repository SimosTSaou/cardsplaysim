#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include <iostream>
int main()
{	
	sf::IpAddress ip;
	const unsigned short port = 23336;

	//some initializations
	sf::TcpSocket socket;
	char connectionType;
	std::size_t received;
	socket.setBlocking(false);
	bool update = false;
	sf::Vector2f prevPos, p2Pos;
	
	//You choose side: server or client
	std::cout << "Enter 's' for server or 'c' for client" << std::endl;
	std::cin >> connectionType;

	//if chose server you start listening to your open port
	if(connectionType == 's')
	{
		sf::TcpListener listener;
		listener.listen(port);
		listener.accept(socket);
	}	
	//else if chose client you connect to the ip and port of the server that the one who is server gave you
	else 
	{	
		std::cout <<"Enter the ip of the server" << std::endl;
		std::cin >> ip;
		while (socket.connect(ip, port) != sf::Socket::Done) {
		}
	}
	
	//make a window
	sf::RenderWindow window(sf::VideoMode(640, 480), "Game");
	sf::Event event;
	window.setFramerateLimit(60);
	//load assets
	sf::Texture texture1, texture2;
	if(!texture1.loadFromFile("assets/textures/mora.png"))
		return 42;
	if(!texture2.loadFromFile("assets/textures/mora2.png"))
		return 43;	
	sf::Sprite sprite1(texture1);
	sf::Sprite sprite2(texture2);
	sprite1.setPosition(0, 0);
	sprite2.setPosition(0, 200);
	if(connectionType == 's')
		prevPos = sprite1.getPosition();
	else 
		prevPos = sprite2.getPosition();
		
	//GAME LOOP
	while(window.isOpen()){
		while(window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
			}
			//this is needed when testing  both client and server on the same computer, so not both windows take the keyboard hits
			if (event.type == sf::Event::GainedFocus)
				update = true;
			else if (event.type == sf::Event::LostFocus)
				update = false;
			
			
			//move your sprite with the keyboard
			
			if(update && connectionType == 's'){
				if (event.type == sf::Event::KeyPressed){
					if(event.key.code == sf::Keyboard::Left)
						sprite1.move(-10, 0);
					if(event.key.code == sf::Keyboard::Right)
						sprite1.move(10, 0);
					if(event.key.code == sf::Keyboard::Up)
						sprite1.move(0, -10);
					if(event.key.code == sf::Keyboard::Down)
						sprite1.move(0, 10);
				}
			} else if(update && connectionType == 'c'){
				if (event.type == sf::Event::KeyPressed){
					if(event.key.code == sf::Keyboard::Left)
						sprite2.move(-10, 0);
					if(event.key.code == sf::Keyboard::Right)
						sprite2.move(10, 0);
					if(event.key.code == sf::Keyboard::Up)
						sprite2.move(0, -10);
					if(event.key.code == sf::Keyboard::Down)
						sprite2.move(0, 10);
				}
			}
		}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && update){
			//sprite1.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
			//std::cout << static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)).x << std::endl;
			//std::cout << static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)).y << std::endl;
		//}
		
		
		
		//Sending and receiving the position of the sprites between computers using packet class
		sf::Packet packet;
		
		if (connectionType == 's'){
			if(prevPos != sprite1.getPosition())
			{
				packet << sprite1.getPosition().x << sprite1.getPosition().y ;
				socket.send(packet);
			}
			socket.receive(packet);
			if(packet >> p2Pos.x >> p2Pos.y)
			{
				sprite2.setPosition(p2Pos);
			}
		} else if (connectionType == 'c'){
			if(prevPos != sprite2.getPosition())
			{
				packet << sprite2.getPosition().x << sprite2.getPosition().y ;
				socket.send(packet);
			}
			socket.receive(packet);
			if(packet >> p2Pos.x >> p2Pos.y)
			{
				sprite1.setPosition(p2Pos);
			}
		}
		//Drawing the sprites
		window.clear(sf::Color::White);
		window.draw(sprite1);
		window.draw(sprite2);
		window.display();
	}
	

	
}
