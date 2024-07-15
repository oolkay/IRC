#include "Server.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include "TextEngine.hpp"
#include "Client.hpp"

#include "Room.hpp"
#include "Define.hpp"
#include "Bot.hpp"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>

#define RPL_ENTRY(password, nick, user) "CAP BOT\nPASS " + password + "\nNICK " + nick + "\nUSER " + user + " 0 * :meral\n"
#define BOT_WELCOME(nick, msg) "PRIVMSG " + nick + " :" + msg + "\r\n"

Bot::Bot(C_STR_REF port, C_STR_REF password) : _port(0), _password(password)
{
	this->_name = "meral";
	try
	{
		this->_port = Utils::ft_stoi(port);
		if (this->_port < 0 || this->_port > 65535)
		{
			throw Exception("Invalid port");
		}
		initSocket();
	}
	catch (const Exception &e)
	{
		throw e;
	}
}

void Bot::initSocket()
{
	this->_socket = socket(AF_INET, SOCK_STREAM, 0); // Create socket 0 for macos and IPPROTO_IP for linux
	if (_socket < 0)
	{
		throw Exception("Socket creation failed");
	}
	else
	{
		TextEngine::green("Socket created successfully", cout) << std::endl;
	}
	memset(&_bot_addr, 0, sizeof(_bot_addr));
	_bot_addr.sin_family = AF_INET;
	_bot_addr.sin_addr.s_addr = INADDR_ANY;
	_bot_addr.sin_port = htons(this->_port);
	if (connect(this->_socket, (struct sockaddr *)&_bot_addr, sizeof(_bot_addr)) < 0)
	{
		throw Exception("Connection failed");
	}
	else
	{
		TextEngine::green("Connection established", cout) << std::endl;
	}
	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		throw Exception("Socket fcntl failed on Bot");
	}
	else
	{
		TextEngine::green("Socket fcntl set successfully", cout) << std::endl;
	}
}



namespace {
    VECT_STR getAnthem() {
        VECT_STR anthem;
        anthem.push_back("Bugün, kapınızı çalacak beklenmedik fırsatlar var.");
		anthem.push_back("Hayatınızı değiştirebilecek bir tesadüf karşılaşması için gözlerinizi açık tutun.");
		anthem.push_back("Bir yabancının gülümsemesi gününüzü aydınlatacak.");
		anthem.push_back("Çabalarınızın ve özverinizin yakında karşılığını alacak.");
		anthem.push_back("Değişime ve yeni başlangıçlara açık olun; büyük şeylere yol açacaklar.");
		anthem.push_back("Küçük bir iyilik, başkalarına mutluluk saçacak.");
		anthem.push_back("İçgüdülerinize güvenin; sizi başarıya götürecekler.");
		anthem.push_back("Yaratıcılığınızın sınırları yok; onu serbest bırakın ve sihir oluşsun.");
		anthem.push_back("Atılacak cesur bir adım, şansın size gülmesini sağlayacak.");
		anthem.push_back("Bugün, etrafınızdaki dünyanın güzellik ve harikalarını takdir etmek için zaman ayırın.");
		anthem.push_back("Uzun zamandır beklediğiniz bir hayal nihayet gerçek olacak.");
		anthem.push_back("Beklenmedik bir haber heyecan ve sevinç getirecek.");
		anthem.push_back("Kalbinizi ve aklınızı dinlemeyi unutmayın; mutluluğun anahtarları onlarda saklı.");
		anthem.push_back("Pozitif tutumunuz bolluk ve refahı çekecek.");
		anthem.push_back("Endişeleri ve şüpheleri bir kenara bırakın; şu anki anı kucaklayın.");
		anthem.push_back("Yeni bir dostluk güzel bir şeye dönüşecek.");
		anthem.push_back("Zorluklar karşısındaki azminiz zaferle sonuçlanacak.");
		anthem.push_back("Macera için bir fırsat bekliyor; coşkuyla kucaklayın.");
		anthem.push_back("Bugün, her şeyin tam olarak olması gerektiğine güvenin.");
		anthem.push_back("Kendinize inanın, çünkü yapabileceklerinizin sınırı yok.");

        return anthem;
    }
}

void	Bot::run(){
	VECT_STR	turkishAnthem = getAnthem();
	Utils::instaWrite(this->_socket, RPL_ENTRY(this->_password, this->_name, "user"));
	while (true){
		fd_set	readfds;
		FD_ZERO(&readfds);
		FD_SET(this->_socket, &readfds);
		struct timeval timeout;
		timeout.tv_sec = 42;
		timeout.tv_usec = 0;

		int ret = select(this->_socket + 1, &readfds, NULL, NULL, &timeout);

		if (ret < 0){
			throw Exception("Select failed");
		}
		else if (ret == 0){
			break;
		}
		else {
			int bytesRead = read(this->_socket, this->_buffer, 1024);
			if (bytesRead < 0){
				throw Exception("Read failed");
			}
			else if (bytesRead == 0){
				break;
			}
			else {
				_buffer[bytesRead] = '\0';
				VECT_STR messages = Utils::ft_split(_buffer, " ");
				TextEngine::blue(_buffer, TextEngine::printTime(cout)) << std::endl;
				if (messages[1] == "PRIVMSG" || messages[1] == "PING"){
					string	nick = Utils::ft_trim(Utils::ft_getNick(messages[0]), "\r\n");
					if (messages[1] == "PING"){
						Utils::instaWrite(this->_socket, "PONG " + messages[2] + "\r\n");
					}
					else {
						Utils::instaWrite(this->_socket, BOT_WELCOME(nick, turkishAnthem[rand() % turkishAnthem.size()]));
					}
				}
			}
		}
	}
	close(this->_socket);
}

Bot::~Bot(){
	if (this->_socket > 0)
		close(this->_socket);
}
