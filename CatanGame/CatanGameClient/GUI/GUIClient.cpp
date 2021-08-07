#include "GUIClient.h"

#include <string_view>
#include <iostream>

#include "Utils/StringUtils.h"
#include "CatanClient/CatanClient.h"

constexpr uint32_t NUMBER_OF_RESOURCES_IN_BOARD = 19;
constexpr std::string_view WINDOW_NAME = "Eli's Catan";

constexpr float WINDOW_HEIGHT = 600.0f;
constexpr float WINDOW_WIDTH = 800.0f;
constexpr float BACKGROUND_WIDTH = 1800.0f;
constexpr float BACKGROUND_HEIGHT = 1581.0f;
constexpr float RESOURCE_HEIGHT = 1074.0f;
constexpr float RESOURCE_WIDTH = 922.0f;
constexpr float RESOURCE_HEIGHT_IN_BACKGROUND = 309.0f;
constexpr float RESOURCE_WIDTH_IN_BACKGROUND = 270.0f;
constexpr float FIRST_POSITION_OF_RESOURCE_X = 230.0f;
constexpr float FIRST_POSITION_OF_RESOURCE_Y = 171.0f;
constexpr float RESOURCE_SCALE_X = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH /
	RESOURCE_WIDTH;
constexpr float RESOURCE_SCALE_Y = WINDOW_HEIGHT * RESOURCE_HEIGHT_IN_BACKGROUND / BACKGROUND_HEIGHT /
	RESOURCE_HEIGHT;
constexpr float FIRST_POSITION_OF_EDGE_X = 240.0f;
constexpr float FIRST_POSITION_OF_EDGE_Y = 270.0f;
constexpr float FIRST_POSITION_OF_STRUCTURE_X = 150.0f;
constexpr float FIRST_POSITION_OF_STRUCTURE_Y = 190.0f;

constexpr uint32_t NONE_PLAYER = 5;

GUIClient::GUIClient()
{
	m_font.loadFromFile("Fonts\\sansation.ttf");
	m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME.data());
	initialize_textures();
	set_background_image();
	initialize_available_resources();
	initialize_available_development_cards();
}

void GUIClient::initialize_textures()
{
	// background
	m_textures.at(static_cast<uint32_t>(TextureTypes::BACKGROUND)).loadFromFile(
		"Images/empty_game_board.png");

	// development cards
	m_textures.at(static_cast<uint32_t>(TextureTypes::KNIGHT_CARD)).loadFromFile(
		"Images/development/knight.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::MONOPOLY_CARD)).loadFromFile(
		"Images/development/monopoly.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::POINT_CARD)).loadFromFile(
		"Images/development/point.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::YEAR_OF_PLENTY_CARD)).loadFromFile(
		"Images/development/abundance.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::ROAD_CARD)).loadFromFile(
		"Images/development/road.png");

	// resources
	m_textures.at(static_cast<uint32_t>(TextureTypes::DESERT)).loadFromFile(
		"Images/resources/desert.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::CLAY)).loadFromFile("Images/resources/clay.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::TREE)).loadFromFile("Images/resources/tree.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::STONE)).loadFromFile("Images/resources/stone.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::WHEAT)).loadFromFile("Images/resources/wheat.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::SHEEP)).loadFromFile("Images/resources/sheep.png");

	m_textures.at(static_cast<uint32_t>(TextureTypes::CLAY_RESOURCE)).loadFromFile(
		"Images/resources_types/clay.jpg");
	m_textures.at(static_cast<uint32_t>(TextureTypes::TREE_RESOURCE)).loadFromFile(
		"Images/resources_types/tree.jpg");
	m_textures.at(static_cast<uint32_t>(TextureTypes::STONE_RESOURCE)).loadFromFile(
		"Images/resources_types/stone.jpg");
	m_textures.at(static_cast<uint32_t>(TextureTypes::WHEAT_RESOURCE)).loadFromFile(
		"Images/resources_types/wheat.jpg");
	m_textures.at(static_cast<uint32_t>(TextureTypes::SHEEP_RESOURCE)).loadFromFile(
		"Images/resources_types/sheep.jpg");

	//numbers 
	m_textures.at(static_cast<uint32_t>(TextureTypes::NUMBER_2)).loadFromFile("Images/numbers/2.tga");
	m_textures.at(static_cast<uint32_t>(TextureTypes::NUMBER_3)).loadFromFile("Images/numbers/3.tga");
	m_textures.at(static_cast<uint32_t>(TextureTypes::NUMBER_4)).loadFromFile("Images/numbers/4.tga");
	m_textures.at(static_cast<uint32_t>(TextureTypes::NUMBER_5)).loadFromFile("Images/numbers/5.tga");
	m_textures.at(static_cast<uint32_t>(TextureTypes::NUMBER_6)).loadFromFile("Images/numbers/6.tga");
	m_textures.at(static_cast<uint32_t>(TextureTypes::NUMBER_8)).loadFromFile("Images/numbers/8.tga");
	m_textures.at(static_cast<uint32_t>(TextureTypes::NUMBER_9)).loadFromFile("Images/numbers/9.tga");
	m_textures.at(static_cast<uint32_t>(TextureTypes::NUMBER_10)).loadFromFile("Images/numbers/10.tga");
	m_textures.at(static_cast<uint32_t>(TextureTypes::NUMBER_11)).loadFromFile("Images/numbers/11.tga");
	m_textures.at(static_cast<uint32_t>(TextureTypes::NUMBER_12)).loadFromFile("Images/numbers/12.tga");

	//dices   
	m_textures.at(static_cast<uint32_t>(TextureTypes::DICE_1)).loadFromFile("Images/dices/dice1.jpg");
	m_textures.at(static_cast<uint32_t>(TextureTypes::DICE_2)).loadFromFile("Images/dices/dice2.jpg");
	m_textures.at(static_cast<uint32_t>(TextureTypes::DICE_3)).loadFromFile("Images/dices/dice3.jpg");
	m_textures.at(static_cast<uint32_t>(TextureTypes::DICE_4)).loadFromFile("Images/dices/dice4.jpg");
	m_textures.at(static_cast<uint32_t>(TextureTypes::DICE_5)).loadFromFile("Images/dices/dice5.jpg");
	m_textures.at(static_cast<uint32_t>(TextureTypes::DICE_6)).loadFromFile("Images/dices/dice6.jpg");

	//objects 
	m_textures.at(static_cast<uint32_t>(TextureTypes::ROBBER)).loadFromFile(
		"Images/objects/catan-robber.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::EDGE_BLUE)).loadFromFile(
		"Images/objects/edge_blue.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::EDGE_GREEN)).loadFromFile(
		"Images/objects/edge_green.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::EDGE_YELLOW)).loadFromFile(
		"Images/objects/edge_yellow.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::EDGE_RED)).loadFromFile(
		"Images/objects/edge_red.png");

	m_textures.at(static_cast<uint32_t>(TextureTypes::SETTLEMENT_BLUE)).loadFromFile(
		"Images/objects/settlement_blue.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::SETTLEMENT_GREEN)).loadFromFile(
		"Images/objects/settlement_green.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::SETTLEMENT_YELLOW)).loadFromFile(
		"Images/objects/settlement_yellow.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::SETTLEMENT_RED)).loadFromFile(
		"Images/objects/settlement_red.png");

	m_textures.at(static_cast<uint32_t>(TextureTypes::CITY_BLUE)).loadFromFile(
		"Images/objects/city_blue.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::CITY_GREEN)).loadFromFile(
		"Images/objects/city_green.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::CITY_YELLOW)).loadFromFile(
		"Images/objects/city_yellow.png");
	m_textures.at(static_cast<uint32_t>(TextureTypes::CITY_RED)).loadFromFile(
		"Images/objects/city_red.png");
}

void GUIClient::set_background_image()
{
	m_background.setTexture(m_textures.at(static_cast<uint32_t>(TextureTypes::BACKGROUND)));
	m_background.setPosition(sf::Vector2f(0.0f, 0.0f));

	m_background.setScale(
		get_background_image_scale(m_textures.at(static_cast<uint32_t>(TextureTypes::BACKGROUND))));
}

sf::Vector2f GUIClient::get_background_image_scale(const sf::Texture& background) const
{
	auto x = static_cast<float>(m_window.getSize().x);
	x /= background.getSize().x;

	auto y = static_cast<float>(m_window.getSize().y);
	y /= background.getSize().y;

	return sf::Vector2f(x, y);
}

void GUIClient::initialize_board_resources(const std::string& resources_data)
{
	bool pos[] = {
		false, false, true, true, true,
		false, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true, false,
		true, true, true, false, false,
	};

	m_board_resources.resize(NUMBER_OF_RESOURCES_IN_BOARD);
	m_board_resources_numbers.resize(NUMBER_OF_RESOURCES_IN_BOARD);
	uint32_t current_pos = 0;
	uint32_t current_resource = 0;
	for (auto& resource_data : CatanUtils::StringUtils::split(resources_data, ";"))
	{
		const auto resource_type = static_cast<CatanUtils::ResourceType>(std::atoi(
			CatanUtils::StringUtils::split(resource_data, ",").at(0).c_str()));
		const uint32_t resource_number = std::atoi(
			CatanUtils::StringUtils::split(resource_data, ",").at(1).c_str());
		if (pos[current_pos])
		{
			const auto i = current_pos / 5;
			const auto j = current_pos % 5;
			initialize_resource_type(current_resource, j, i, resource_type);
			initialize_resource_number(current_resource, j, i, resource_number);
			current_resource++;
		}
		current_pos++;
	}
}

void GUIClient::initialize_resource_type(const uint32_t resource_index, const uint32_t x,
                                         const uint32_t y, const CatanUtils::ResourceType resource_type)
{
	switch (resource_type)
	{
	case CatanUtils::ResourceType::CLAY:
		initialize_resource_type_sprite(resource_index, x, y, TextureTypes::CLAY);
		break;
	case CatanUtils::ResourceType::SHEEP:
		initialize_resource_type_sprite(resource_index, x, y, TextureTypes::SHEEP);
		break;
	case CatanUtils::ResourceType::WHEAT:
		initialize_resource_type_sprite(resource_index, x, y, TextureTypes::WHEAT);
		break;
	case CatanUtils::ResourceType::NONE:
		initialize_resource_type_sprite(resource_index, x, y, TextureTypes::DESERT);
		break;
	case CatanUtils::ResourceType::STONE:
		initialize_resource_type_sprite(resource_index, x, y, TextureTypes::STONE);
		break;
	case CatanUtils::ResourceType::TREE:
		initialize_resource_type_sprite(resource_index, x, y, TextureTypes::TREE);
		break;
	default:
		break;
	}
}

void GUIClient::initialize_resource_type_sprite(const uint32_t resource_index, const uint32_t x,
                                                const uint32_t y, const TextureTypes resource_type)
{
	m_board_resources.at(resource_index).setTexture(m_textures.at(static_cast<uint32_t>(resource_type)));

	m_board_resources.at(resource_index).setScale(RESOURCE_SCALE_X, RESOURCE_SCALE_Y);

	const auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
	const auto size_of_resource_y = WINDOW_HEIGHT * RESOURCE_HEIGHT_IN_BACKGROUND / BACKGROUND_HEIGHT;

	const auto first_position_in_board_x = FIRST_POSITION_OF_RESOURCE_X * WINDOW_WIDTH /
		BACKGROUND_WIDTH;
	const auto first_position_in_board_y = FIRST_POSITION_OF_RESOURCE_Y * WINDOW_HEIGHT /
		BACKGROUND_HEIGHT;

	const auto x1 = first_position_in_board_x + (x - 1.0f + 0.5f * y) * size_of_resource_x;
	const auto y1 = first_position_in_board_y + y * size_of_resource_y * 0.757f;

	m_board_resources.at(resource_index).setPosition(x1, y1);
}

void GUIClient::initialize_resource_number(const uint32_t resource_index, const uint32_t x,
                                           const uint32_t y, const uint32_t resource_number)
{
	switch (resource_number)
	{
	case 0:
		break;
	case 2:
		initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_2);
		break;
	case 3:
		initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_3);
		break;
	case 4:
		initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_4);
		break;
	case 5:
		initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_5);
		break;
	case 6:
		initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_6);
		break;
	case 8:
		initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_8);
		break;
	case 9:
		initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_9);
		break;
	case 10:
		initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_10);
		break;
	case 11:
		initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_11);
		break;
	case 12:
		initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_12);
		break;
	default:
		break;
	}
}

void GUIClient::initialize_resource_number_sprite(const uint32_t resource_index, const uint32_t x,
                                                  const uint32_t y, const TextureTypes resource_number)
{
	m_board_resources_numbers.at(resource_index).setTexture(
		m_textures.at(static_cast<uint32_t>(resource_number)));

	m_board_resources_numbers.at(resource_index).setScale(RESOURCE_SCALE_X - 0.05f,
	                                                      RESOURCE_SCALE_Y - 0.05f);

	const auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
	const auto size_of_resource_y = WINDOW_HEIGHT * RESOURCE_HEIGHT_IN_BACKGROUND / BACKGROUND_HEIGHT;

	const auto first_position_in_board_x = FIRST_POSITION_OF_RESOURCE_X * WINDOW_WIDTH /
		BACKGROUND_WIDTH;
	const auto first_position_in_board_y = FIRST_POSITION_OF_RESOURCE_Y * WINDOW_HEIGHT /
		BACKGROUND_HEIGHT;

	const auto x1 = first_position_in_board_x + (x - 1.0f + 0.5f * y) * size_of_resource_x;
	const auto y1 = first_position_in_board_y + y * size_of_resource_y * 0.757f;

	m_board_resources_numbers.at(resource_index).setPosition(x1 + 35.0f, y1 + 40.0f);
}

void GUIClient::initialize_board_robber(const std::string& robber_location)
{
	const uint32_t x = std::stoi(CatanUtils::StringUtils::split(robber_location, ",").at(0));
	const uint32_t y = std::stoi(CatanUtils::StringUtils::split(robber_location, ",").at(1));

	m_robber.setTexture(m_textures.at(static_cast<uint32_t>(TextureTypes::ROBBER)));

	m_robber.setScale(RESOURCE_SCALE_X + 0.1f, RESOURCE_SCALE_Y + 0.1f);

	const auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
	const auto size_of_resource_y = WINDOW_HEIGHT * RESOURCE_HEIGHT_IN_BACKGROUND / BACKGROUND_HEIGHT;

	const auto first_position_in_board_x = FIRST_POSITION_OF_RESOURCE_X * WINDOW_WIDTH /
		BACKGROUND_WIDTH;
	const auto first_position_in_board_y = FIRST_POSITION_OF_RESOURCE_Y * WINDOW_HEIGHT /
		BACKGROUND_HEIGHT;

	const auto x1 = first_position_in_board_x + (x - 1.0f + 0.5f * y) * size_of_resource_x;
	const auto y1 = first_position_in_board_y + y * size_of_resource_y * 0.757f;

	m_robber.setPosition(x1 + 35.0f, y1 + 40.0f);
}

void GUIClient::initialize_dices()
{
	m_dice_1.setTexture(m_textures.at(static_cast<uint32_t>(TextureTypes::DICE_1)));
	m_dice_2.setTexture(m_textures.at(static_cast<uint32_t>(TextureTypes::DICE_1)));

	m_dice_1.setScale(0.3f, 0.3f);
	m_dice_2.setScale(0.3f, 0.3f);

	m_dice_1.setPosition(0, 0);
	m_dice_2.setPosition(50, 0);
}

void GUIClient::initialize_available_resources()
{
	for (size_t i = 0; i < m_available_resources_images.size(); i++)
	{
		m_available_resources_images.at(i).setTexture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::WHEAT_RESOURCE) + i));
		m_available_resources_images.at(i).setPosition(m_window.getSize().x - 80.0f, 20.0f + i * 25);
		m_available_resources_images.at(i).setScale(0.5f, 0.5f);

		m_available_resources_texts.at(i).setFont(m_font);
		m_available_resources_texts.at(i).setFillColor(sf::Color::Black);
		m_available_resources_texts.at(i).setString("0");
		m_available_resources_texts.at(i).setPosition(m_window.getSize().x - 30.0f, 15.0f + i * 25);
		m_available_resources_texts.at(i).setCharacterSize(24);
		m_available_resources_texts.at(i).setStyle(sf::Text::Bold);
	}
}

void GUIClient::initialize_available_development_cards()
{
	for (size_t i = 0; i < m_available_resources_images.size(); i++)
	{
		m_available_development_cards_images.at(i).setTexture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::KNIGHT_CARD) + i));
		m_available_development_cards_images.at(i).setPosition(m_window.getSize().x - 100.0f,
		                                                       440.0f + i * 25);
		m_available_development_cards_images.at(i).setScale(0.05f, 0.035f);

		m_available_development_cards_texts.at(i).setFont(m_font);
		m_available_development_cards_texts.at(i).setFillColor(sf::Color::Black);
		m_available_development_cards_texts.at(i).setString("0");
		m_available_development_cards_texts.at(i).setPosition(m_window.getSize().x - 30.0f,
		                                                      435.0f + i * 25);
		m_available_development_cards_texts.at(i).setCharacterSize(24);
		m_available_development_cards_texts.at(i).setStyle(sf::Text::Bold);
	}
}

void GUIClient::start_game()
{
	render_loop();
}

void GUIClient::create_catan_board(const std::string& board_data)
{
	auto all_data = CatanUtils::StringUtils::split(board_data, "\n");
	initialize_board_resources(all_data.at(0));
	initialize_board_robber(all_data.at(3));
	initialize_dices();
}

void GUIClient::update_board(const std::string& board_data)
{
	auto all_data = CatanUtils::StringUtils::split(board_data, "\n");
	fetch_structures(all_data.at(1));
	fetch_edges(all_data.at(2));
	set_robber_position(all_data.at(3));
}

void GUIClient::fetch_structures(const std::string& structures)
{
	uint32_t x = 0;
	uint32_t y = -1;

	auto current_structure = CatanUtils::StringUtils::split(structures, ";");
	for (const auto& structure : current_structure)
	{
		if (x % NUMBER_OF_NODES_IN_A_ROW == 0)
		{
			y++;
			x = 0;
		}
		auto structure_data = CatanUtils::StringUtils::split(structure, ",");
		const uint32_t structure_player = std::stoi(structure_data.at(1));
		const auto structure_type = static_cast<CatanUtils::StructureType>(std::stoi(
			structure_data.at(0)));
		if (structure_type == CatanUtils::StructureType::NONE)
		{
			x++;
			continue;
		}
			// TODO: Think if I need to throw here
		else if (structure_type == CatanUtils::StructureType::SETTLEMENT)
		{
			add_new_settlement(x, y, structure_player);
		}
		else if (structure_type == CatanUtils::StructureType::CITY)
		{
			upgrade_settlement_to_city(x, y, structure_player);
		}
		x++;
	}
}

void GUIClient::add_new_settlement(const uint32_t x, const uint32_t y, const uint32_t player_number)
{
	switch (static_cast<CatanUtils::PlayerType>(player_number))
	{
	case CatanUtils::PlayerType::BLUE:
		m_board_structures.at(y).at(x).set_texture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::SETTLEMENT_BLUE)));
		break;
	case CatanUtils::PlayerType::GREEN:
		m_board_structures.at(y).at(x).set_texture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::SETTLEMENT_GREEN)));
		break;
	case CatanUtils::PlayerType::RED:
		m_board_structures.at(y).at(x).set_texture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::SETTLEMENT_RED)));
		break;
	case CatanUtils::PlayerType::YELLOW:
		m_board_structures.at(y).at(x).set_texture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::SETTLEMENT_YELLOW)));
		break;
	default:
		break;
	}

	m_board_structures.at(y).at(x).set_scale(0.09f, 0.09f);
	initialize_structure_position(m_board_structures.at(y).at(x), x, y);
}

void GUIClient::initialize_structure_position(NodeSprite& structure, uint32_t x, const uint32_t y)
{
	// TODO fix this function - it looks very bad
	const auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
	const auto size_of_resource_y = WINDOW_HEIGHT * RESOURCE_HEIGHT_IN_BACKGROUND / BACKGROUND_HEIGHT;

	/*float shift_right = NUMBER_OF_EDGES_IN_A_COLUMN / 2;
	shift_right -= y;
	shift_right = std::max(shift_right, 0.0f);
	shift_right = x - shift_right;
	shift_right /= 2;

	float temp = NUMBER_OF_EDGES_IN_A_COLUMN / 2.0f - y;
	temp = std::abs(std::floor(temp)) / 4;
	shift_right += temp;

	auto x1 = (FIRST_POSITION_OF_STRUCTURE_X * WINDOW_WIDTH / BACKGROUND_WIDTH) + size_of_resource_x * shift_right;
	auto y1 = (FIRST_POSITION_OF_STRUCTURE_Y * WINDOW_HEIGHT / BACKGROUND_HEIGHT) + size_of_resource_y * (y * 0.757f);
	if (y == 0) {
	    x--;
	}
	if (x % 2 == 1) {
	    y1 += 9.0f;
	}
	else {
	    y1 -= 9.0f;
	}*/
	auto x1 = (FIRST_POSITION_OF_STRUCTURE_X * WINDOW_WIDTH / BACKGROUND_WIDTH);
	auto y1 = (FIRST_POSITION_OF_STRUCTURE_Y * WINDOW_HEIGHT / BACKGROUND_HEIGHT);

	float a = 0;

	if (y == 0)
	{
		a = 1.0f;
		x -= 4;
	}
	if (y == 1)
	{
		a = 0.5f;
		x -= 3;
	}
	if (y == 2)
	{
		x -= 1;
	}
	if (y == 4)
	{
		a = 0.5f;
	}
	if (y == 5)
	{
		a = 1.0f;
	}
	x1 += size_of_resource_x * (a + x / 2.0f);
	if ((y == 0 && x % 2 == 1))
	{
		y1 -= 30.0f;
	}
	else if (y < NUMBER_OF_NODES_IN_A_COLUMN / 2 && x % 2 == 1)
	{
		y1 -= 30.0f;
	}
	else if (y >= NUMBER_OF_NODES_IN_A_COLUMN / 2 && x % 2 == 0)
	{
		y1 -= 30.0f;
	}
	y1 += y * size_of_resource_y * 0.757f;
	structure.set_position(x1, y1);
}

void GUIClient::upgrade_settlement_to_city(const uint32_t x, const uint32_t y,
                                           const uint32_t player_number)
{
	switch (static_cast<CatanUtils::PlayerType>(player_number))
	{
	case CatanUtils::PlayerType::BLUE:
		m_board_structures.at(y).at(x).set_texture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::CITY_BLUE)));
		break;
	case CatanUtils::PlayerType::YELLOW:
		m_board_structures.at(y).at(x).set_texture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::CITY_YELLOW)));
		break;
	case CatanUtils::PlayerType::RED:
		m_board_structures.at(y).at(x).set_texture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::CITY_RED)));
		break;
	case CatanUtils::PlayerType::GREEN:
		m_board_structures.at(y).at(x).set_texture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::CITY_GREEN)));
		break;
	default:
		break;
	}
}

void GUIClient::fetch_edges(const std::string& edges)
{
	uint32_t x = 0;
	uint32_t y = -1;

	auto current_edges = CatanUtils::StringUtils::split(edges, ";");
	for (const auto& edge : current_edges)
	{
		if (x % NUMBER_OF_EDGES_IN_A_ROW == 0)
		{
			y++;
			x = 0;
		}
		const uint32_t edge_player = std::stoi(edge);
		if (edge_player == NONE_PLAYER || m_board_edges.at(y).at(x).getTexture() != nullptr)
		{
			x++;
			continue;
		}
		add_new_edge(x, y, edge_player);
		x++;
	}
}

void GUIClient::add_new_edge(const uint32_t x, const uint32_t y, const uint32_t player_number)
{
	switch (static_cast<CatanUtils::PlayerType>(player_number))
	{
	case CatanUtils::PlayerType::BLUE:
		m_board_edges.at(y).at(x).setTexture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::EDGE_BLUE)));
		break;
	case CatanUtils::PlayerType::GREEN:
		m_board_edges.at(y).at(x).setTexture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::EDGE_GREEN)));
		break;
	case CatanUtils::PlayerType::RED:
		m_board_edges.at(y).at(x).setTexture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::EDGE_RED)));
		break;
	case CatanUtils::PlayerType::YELLOW:
		m_board_edges.at(y).at(x).setTexture(
			m_textures.at(static_cast<uint32_t>(TextureTypes::EDGE_YELLOW)));
		break;
	default:
		break;
	}

	m_board_edges.at(y).at(x).setScale(0.6f, 0.6f);
	initialize_edge_position(m_board_edges.at(y).at(x), x, y);
}

void GUIClient::initialize_edge_position(sf::Sprite& edge, uint32_t x, const uint32_t y)
{
	const auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
	const auto size_of_resource_y = WINDOW_HEIGHT * RESOURCE_HEIGHT_IN_BACKGROUND / BACKGROUND_HEIGHT;
	float shift_right = NUMBER_OF_EDGES_IN_A_COLUMN / 2;
	shift_right -= y;
	shift_right = std::max(shift_right, 0.0f);
	shift_right = x - shift_right;
	shift_right /= 2;

	auto temp = NUMBER_OF_EDGES_IN_A_COLUMN / 2.0f - y;
	temp = std::abs(std::floor(temp)) / 4;
	shift_right += temp;

	auto x1 = (FIRST_POSITION_OF_EDGE_X * WINDOW_WIDTH / BACKGROUND_WIDTH) + size_of_resource_x *
		shift_right;
	auto y1 = (FIRST_POSITION_OF_EDGE_Y * WINDOW_HEIGHT / BACKGROUND_HEIGHT) + size_of_resource_y * (y /
		2 * 0.757f);


	if (y % 2 == 1)
	{
		edge.setRotation(90);
	}
	else
	{
		if (y == 0)
		{
			x--;
			x1 += size_of_resource_x / 2;
		}
		if (x % 2 == 1)
		{
			x1 += 14.0f;
			y1 -= 28.0f;
			edge.setRotation(153);
		}
		else
		{
			x1 += 13.0f;
			y1 -= 9.0f;
			edge.setRotation(-153);
		}
	}

	edge.setPosition(x1, y1);
}

void GUIClient::set_robber_position(const std::string& robber_location)
{
	const uint32_t x = std::stoi(CatanUtils::StringUtils::split(robber_location, ",").at(1));
	const uint32_t y = std::stoi(CatanUtils::StringUtils::split(robber_location, ",").at(0));

	const auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
	const auto size_of_resource_y = WINDOW_HEIGHT * RESOURCE_HEIGHT_IN_BACKGROUND / BACKGROUND_HEIGHT;

	const auto first_position_in_board_x = FIRST_POSITION_OF_RESOURCE_X * WINDOW_WIDTH /
		BACKGROUND_WIDTH;
	const auto first_position_in_board_y = FIRST_POSITION_OF_RESOURCE_Y * WINDOW_HEIGHT /
		BACKGROUND_HEIGHT;

	const auto x1 = first_position_in_board_x + (x - 1.0f + 0.5f * y) * size_of_resource_x;
	const auto y1 = first_position_in_board_y + y * size_of_resource_y * 0.757f;

	m_robber.setPosition(x1 + 35.0f, y1 + 40.0f);
}

void GUIClient::update_dices(const uint32_t first_dice, const uint32_t second_dice)
{
	m_dice_1.setTexture(m_textures.at(first_dice + static_cast<uint32_t>(TextureTypes::DICE_1) - 1));
	m_dice_2.setTexture(m_textures.at(second_dice + static_cast<uint32_t>(TextureTypes::DICE_1) - 1));
}

void GUIClient::update_available_resources(
	const std::unordered_map<CatanUtils::ResourceType, uint32_t>& resources)
{
	m_available_resources_texts.at(static_cast<uint32_t>(CatanUtils::ResourceType::CLAY) - 1).setString(
		std::to_string(resources.at(CatanUtils::ResourceType::CLAY)));
	m_available_resources_texts.at(static_cast<uint32_t>(CatanUtils::ResourceType::SHEEP) - 1).setString(
		std::to_string(resources.at(CatanUtils::ResourceType::SHEEP)));
	m_available_resources_texts.at(static_cast<uint32_t>(CatanUtils::ResourceType::STONE) - 1).setString(
		std::to_string(resources.at(CatanUtils::ResourceType::STONE)));
	m_available_resources_texts.at(static_cast<uint32_t>(CatanUtils::ResourceType::WHEAT) - 1).setString(
		std::to_string(resources.at(CatanUtils::ResourceType::WHEAT)));
	m_available_resources_texts.at(static_cast<uint32_t>(CatanUtils::ResourceType::TREE) - 1).setString(
		std::to_string(resources.at(CatanUtils::ResourceType::TREE)));
}

void GUIClient::update_available_development_cards(
	const std::unordered_map<CatanUtils::DevelopmentCards, uint32_t>& development_cards)
{
	m_available_development_cards_texts.
		at(static_cast<uint32_t>(CatanUtils::DevelopmentCards::POINT_CARD)).setString(
			std::to_string(development_cards.at(CatanUtils::DevelopmentCards::POINT_CARD)));
	m_available_development_cards_texts.
		at(static_cast<uint32_t>(CatanUtils::DevelopmentCards::MONOPOL_CARD)).setString(
			std::to_string(development_cards.at(CatanUtils::DevelopmentCards::MONOPOL_CARD)));
	m_available_development_cards_texts.
		at(static_cast<uint32_t>(CatanUtils::DevelopmentCards::ABUNDANCE_CARD)).setString(
			std::to_string(development_cards.at(CatanUtils::DevelopmentCards::ABUNDANCE_CARD)));
	m_available_development_cards_texts.
		at(static_cast<uint32_t>(CatanUtils::DevelopmentCards::KNIGHT_CARD)).setString(
			std::to_string(development_cards.at(CatanUtils::DevelopmentCards::KNIGHT_CARD)));
	m_available_development_cards_texts.
		at(static_cast<uint32_t>(CatanUtils::DevelopmentCards::ROAD_CARD)).setString(
			std::to_string(development_cards.at(CatanUtils::DevelopmentCards::ROAD_CARD)));
}

void GUIClient::render_loop()
{
	while (m_window.isOpen())
	{
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_window.close();
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				std::cout << "########### mouse : " << sf::Mouse::getPosition(m_window).x << "," <<
					sf::Mouse::getPosition(m_window).y << std::endl;
			}
		}
		draw();
	}
}

void GUIClient::draw()
{
	m_window.clear(sf::Color::White);
	m_window.draw(m_background);
	for (const auto& board_resource : m_board_resources)
	{
		m_window.draw(board_resource);
	}
	for (const auto& board_resources_number : m_board_resources_numbers)
	{
		m_window.draw(board_resources_number);
	}
	for (const auto& board_structure_row : m_board_structures)
	{
		for (const auto& board_structure : board_structure_row)
		{
			m_window.draw(board_structure.get_sprite());
		}
	}
	for (auto& board_edge_row : m_board_edges)
	{
		for (const auto& board_edge : board_edge_row)
		{
			m_window.draw(board_edge);
		}
	}
	for (size_t i = 0; i < m_available_resources_images.size(); i++)
	{
		m_window.draw(m_available_resources_images.at(i));
		m_window.draw(m_available_resources_texts.at(i));
	}
	for (size_t i = 0; i < m_available_development_cards_images.size(); i++)
	{
		m_window.draw(m_available_development_cards_images.at(i));
		m_window.draw(m_available_development_cards_texts.at(i));
	}

	m_window.draw(m_robber);
	m_window.draw(m_dice_1);
	m_window.draw(m_dice_2);
	m_window.display();
}
