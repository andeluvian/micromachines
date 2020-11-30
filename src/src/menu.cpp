#include "menu.hpp"

Menu::Menu() :
	width(WIDTH), height(HEIGHT)
{
	load(ButtonTexture::EMPTYBUTTON, "button.png");
	load(ButtonTexture::SELECTEDBUTTON, "selectedButton.png");
	load(ButtonTexture::BANNEDBUTTON, "bannedButton.png");
	if (!font.loadFromFile("../images/fonts/open-sans/OpenSans-Regular.ttf"))
	{
		if (!font.loadFromFile("../../images/fonts/open-sans/OpenSans-Regular.ttf"))
			std::cout << "Failed to load fonts." << std::endl;
	}
}

Menu::~Menu()
{
}

/// Load a texture and store it to menuTextures.
void Menu::load(ButtonTexture ID, const std::string& filename)
{

	std::unique_ptr < sf::Texture> texture(new sf::Texture());
	if (!texture->loadFromFile("../images/" + filename)) {
		if (!texture->loadFromFile("../../images/" + filename)) {
			std::cout << "Couldn't find " << filename << std::endl;
		}
	}

	menuTextures.insert(std::make_pair(ID, std::move(texture)));

}


/// Load the sprite for a button, set its position and add it to menuSprites.
/// newID allows you to use multiples of the same texture and have different implementations for them.
/// For example:	load(ButtonTexture::EMPTYBUTTON, ButtonTexture::RACE, 0.2f * width, 0.2f * height)
///					load(ButtonTexture::EMPTYBUTTON, ButtonTexture::TIMETRIAL, 0.2f * width, 0.35 * height)
/// This is more efficient than calling the other load with different ButtonTextures for everything.
void Menu::loadSprite(ButtonTexture ID, ButtonTexture newID, float posx, float posy)
{

	sf::Sprite button;
	button.setTexture(getTexture(ID));
	button.setPosition(posx, posy);
	menuSprites.insert({ newID, button });
}

/// Takes enum class ButtonTexture::BUTTONNAME as a parameter
/// Returns a reference to that texture.
sf::Texture& Menu::getTexture(ButtonTexture ID)
{
	auto found = menuTextures.find(ID);
	return *found->second;
}

/// Const version of the getTexture function.
/// Takes enum class ButtonTexture::BUTTONNAME as a parameter
/// Returns a reference to that texture.
const sf::Texture& Menu::getTexture(ButtonTexture ID) const

{
	auto found = menuTextures.find(ID);
	return *found->second;
}

/// Sets a string and text to a button, centering the text.
/// Note that text centering with large font sizes doesn't work properly.
void Menu::setText(ButtonTexture ID, const std::string& str)
{
	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setStyle(sf::Text::Bold);

#ifdef _WIN32
	text.setFillColor(sf::Color(255, 242, 0));
#else
	text.setColor(sf::Color(255, 242, 0));
#endif

	text.setCharacterSize(20);
	sf::FloatRect textRect = text.getGlobalBounds();
	text.setOrigin(textRect.width / 2, textRect.height / 2);
	sf::FloatRect buttonRect = menuSprites[ID].getGlobalBounds();
	text.setPosition(buttonRect.left + buttonRect.width *0.5f, buttonRect.top + buttonRect.height *0.5f);

	menuStrings.push_back(text);
}

/// Set a string with absolute position.
void Menu::setText(float x, float y, const std::string& str)
{
	sf::Text text;
	text.setFont(font);
	text.setString(str);
	//text.setStyle(sf::Text::Bold);

#ifdef _WIN32
	text.setFillColor(sf::Color(255, 242, 0));
#else
	text.setColor(sf::Color(255, 242, 0));
#endif

	text.setCharacterSize(20);
	sf::FloatRect textRect = text.getGlobalBounds();
	text.setOrigin(textRect.width / 2, textRect.height / 2);
	text.setPosition(x, y);

	menuStrings.push_back(text);
}

void Menu::setBanned(ButtonTexture ID, bool value)
{
	if (value == true)
	{
		menuSprites[ID].setTexture(getTexture(ButtonTexture::BANNEDBUTTON));
		bannedButtons.push_back(ID);
	}
	if (value == false)
	{
		menuSprites[ID].setTexture(getTexture(ButtonTexture::EMPTYBUTTON));
		bannedButtons.push_back(ID);
	}

}

bool Menu::getBanned(ButtonTexture ID)
{
	auto found = std::find(bannedButtons.begin(), bannedButtons.end(), ID);
	return (found != bannedButtons.end());

}

void Menu::setSelected(ButtonTexture ID, bool value)
{
	if (getBanned(ID))
		return;

	if (value == true)
	{
		if (!getSelected(ID))
		{
			menuSprites[ID].setTexture(getTexture(ButtonTexture::SELECTEDBUTTON));
			selectedButtons.push_back(ID);
			pressedFlag = true;
		}
	}
	if (value == false)
	{
		auto found = std::find(selectedButtons.begin(), selectedButtons.end(), ID);
		if (found != selectedButtons.end())
		{
			selectedButtons.erase(found);
			menuSprites[ID].setTexture(getTexture(ButtonTexture::EMPTYBUTTON));
		}
	}
}

bool Menu::getSelected(ButtonTexture ID)
{
	auto found = std::find(selectedButtons.begin(), selectedButtons.end(), ID);
	return (found != selectedButtons.end());
}

/// Draws the items in the menuSprites map. Called every cycle.
void Menu::drawHandler(sf::RenderWindow& window)
{

	window.clear(bgColor);
	for (auto i : menuSprites)
		window.draw(i.second);
	for (auto i : menuStrings)
		window.draw(i);
}

void Menu::updateSound(SoundHandler& soundHandler)
{
	if (pressedFlag)
	{
		if (!soundHandler.isPlaying(SoundType::BUTTON))
			soundHandler.playSound(SoundType::BUTTON);
		pressedFlag = false;
	}
}