#include "GUIClient.h"

#include <string_view>

#include "StringUtils/StringUtils.h"
#include "CatanClient/CatanClient.h"

constexpr uint32_t NUMBER_OF_TEXTURE_TYPES = 47;
constexpr uint32_t NUMBER_OF_RESOURCES_IN_BOARD = 19;
constexpr std::string_view WINDOW_NAME = "Eli's Catan";

constexpr float WINDOW_HIGHT = 600.0f;
constexpr float WINDOW_WIDTH = 800.0f;
constexpr float BACKGROUND_WIDTH = 1800.0f;
constexpr float BACKGROUND_HIGHT = 1581.0f;
constexpr float RESOURCE_HIGHT = 1074.0f;
constexpr float RESOURCE_WIDTH = 922.0f;
constexpr float RESOURCE_HIGHT_IN_BACKGROUND = 309.0f;
constexpr float RESOURCE_WIDTH_IN_BACKGROUND = 270.0f;
constexpr float FIRST_POSITION_X = 230.0f;
constexpr float FIRST_POSITION_Y = 171.0f;
constexpr float RESOURCE_SCALE_X = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH / RESOURCE_WIDTH;
constexpr float RESOURCE_SCALE_Y = WINDOW_HIGHT * RESOURCE_HIGHT_IN_BACKGROUND / BACKGROUND_HIGHT / RESOURCE_HIGHT;

GUIClient::GUIClient():
    m_textures(NUMBER_OF_TEXTURE_TYPES)
{
    m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HIGHT), WINDOW_NAME.data());
    initialize_textures();
    set_background_image();
}

void GUIClient::initialize_textures() {
    // background
    m_textures[static_cast<uint32_t>(TextureTypes::BACKGROUND)].loadFromFile("Images/empty_game_board.png");
    
    // resources
    m_textures[static_cast<uint32_t>(TextureTypes::DESERT)].loadFromFile("Images/resources/desert.png");
    m_textures[static_cast<uint32_t>(TextureTypes::CLAY)].loadFromFile("Images/resources/clay.png");
    m_textures[static_cast<uint32_t>(TextureTypes::TREE)].loadFromFile("Images/resources/tree.png");
    m_textures[static_cast<uint32_t>(TextureTypes::STONE)].loadFromFile("Images/resources/stone.png");
    m_textures[static_cast<uint32_t>(TextureTypes::WHEAT)].loadFromFile("Images/resources/wheat.png");
    m_textures[static_cast<uint32_t>(TextureTypes::SHEEP)].loadFromFile("Images/resources/sheep.png");

    //numbers
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_2)].loadFromFile("Images/numbers/2.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_3)].loadFromFile("Images/numbers/3.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_4)].loadFromFile("Images/numbers/4.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_5)].loadFromFile("Images/numbers/5.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_6)].loadFromFile("Images/numbers/6.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_8)].loadFromFile("Images/numbers/8.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_9)].loadFromFile("Images/numbers/9.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_10)].loadFromFile("Images/numbers/10.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_11)].loadFromFile("Images/numbers/11.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_12)].loadFromFile("Images/numbers/12.tga");
    

}

void GUIClient::set_background_image() {
    m_background.setTexture(m_textures.at(static_cast<uint32_t>(TextureTypes::BACKGROUND)));
    m_background.setPosition(sf::Vector2f(0.0f, 0.0f));

    m_background.setScale(get_background_image_scale(m_textures.at(static_cast<uint32_t>(TextureTypes::BACKGROUND))));
}

sf::Vector2f GUIClient::get_background_image_scale(const sf::Texture& background) const {

    float x;
    float y;

    x = static_cast<float>(m_window.getSize().x);
    x /= background.getSize().x;

    y = static_cast<float>(m_window.getSize().y);
    y /= background.getSize().y;

    return sf::Vector2f(x, y);
}

void GUIClient::create_board(const std::string& board_data) {

    auto all_data = split(board_data, "\n");
    initialize_board_resources(all_data[0]);
}

void GUIClient::initialize_board_resources(const std::string& resources_data) {

    bool pos[] = {
        0, 0, 1, 1, 1,
        0, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 0,
        1, 1, 1, 0, 0
    };

    m_board_resources.resize(NUMBER_OF_RESOURCES_IN_BOARD);
    m_board_resources_numbers.resize(NUMBER_OF_RESOURCES_IN_BOARD);
    uint32_t current_pos = 0;
    uint32_t current_resource = 0;
    for (auto resource_data : split(resources_data, ";")) {
        auto resource_type = static_cast<ResourceType>(atoi(split(resource_data, ",")[0].c_str()));
        uint32_t resource_number = atoi(split(resource_data, ",")[1].c_str());
        if (pos[current_pos]) {
            uint32_t i = current_pos / 5;
            uint32_t j = current_pos % 5;
            initialize_resource_type(current_resource, j, i, resource_type);
            initialize_resource_number(current_resource, j, i, resource_number);
            current_resource++;
        }
        current_pos++;
    }
}

void GUIClient::initialize_resource_type(uint32_t resource_index, uint32_t x, uint32_t y, ResourceType resource_type) {
    switch (resource_type)
    {
    case ResourceType::CLAY:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::CLAY);
        break;
    case ResourceType::SHEEP:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::SHEEP);
        break;
    case ResourceType::WHEAT:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::WHEAT);
        break;
    case ResourceType::NONE:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::DESERT);
        break;
    case ResourceType::STONE:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::STONE);
        break;
    case ResourceType::TREE:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::TREE);
        break;
    default:
        break;
    }
}

void GUIClient::initialize_resource_type_sprite(uint32_t resource_index, uint32_t x, uint32_t y, TextureTypes resource_type) {
    m_board_resources[resource_index].setTexture(m_textures[static_cast<uint32_t>(resource_type)]);

    m_board_resources[resource_index].setScale(RESOURCE_SCALE_X, RESOURCE_SCALE_Y);

    auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
    auto size_of_resource_y = WINDOW_HIGHT * RESOURCE_HIGHT_IN_BACKGROUND / BACKGROUND_HIGHT;

    float first_position_in_board_x = FIRST_POSITION_X * WINDOW_WIDTH / BACKGROUND_WIDTH;
    float first_position_in_board_y = FIRST_POSITION_Y * WINDOW_HIGHT / BACKGROUND_HIGHT;

    auto x1 = first_position_in_board_x + (x - 1.0f + 0.5f * y) * size_of_resource_x;
    auto y1 = first_position_in_board_y + y * size_of_resource_y * 0.757f;

    m_board_resources[resource_index].setPosition(x1, y1);
}

void GUIClient::initialize_resource_number(uint32_t resource_index, uint32_t x, uint32_t y, uint32_t resource_number) {
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

void GUIClient::initialize_resource_number_sprite(uint32_t resource_index, uint32_t x, uint32_t y, TextureTypes resource_number) {
    m_board_resources_numbers[resource_index].setTexture(m_textures[static_cast<uint32_t>(resource_number)]);

    m_board_resources_numbers[resource_index].setScale(RESOURCE_SCALE_X, RESOURCE_SCALE_Y);

    auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
    auto size_of_resource_y = WINDOW_HIGHT * RESOURCE_HIGHT_IN_BACKGROUND / BACKGROUND_HIGHT;

    float first_position_in_board_x = FIRST_POSITION_X * WINDOW_WIDTH / BACKGROUND_WIDTH;
    float first_position_in_board_y = FIRST_POSITION_Y * WINDOW_HIGHT / BACKGROUND_HIGHT;

    auto x1 = first_position_in_board_x + (x - 1.0f + 0.5f * y) * size_of_resource_x;
    auto y1 = first_position_in_board_y + y * size_of_resource_y * 0.757f;

    m_board_resources_numbers[resource_index].setPosition(x1, y1);
}

void GUIClient::render_loop() {
    
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();

        }

        m_window.clear(sf::Color::Green);
        m_window.draw(m_background);
        for (size_t i = 0; i < m_board_resources.size(); i++) {
            m_window.draw(m_board_resources[i]);
        }
        for (size_t i = 0; i < m_board_resources_numbers.size(); i++) {
            m_window.draw(m_board_resources_numbers[i]);
        }
        m_window.display();
    }
}

void GUIClient::start_game() {
    render_loop();
}

GUIClient::~GUIClient(){
}
