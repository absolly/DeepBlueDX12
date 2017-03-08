#include "PlayerFishAbilityBehaviour.h"
#include "Content\GameObjects\PlayerFishFlock.hpp"
#include "mge/materials/GPUinstancingMaterial.hpp"

#include "Content\GameObjects\Player.h"
#include "mge\core\SoundManager.hpp"
#include "mge\core\Random.h"
#include "Content\Core\Input.h"
#include "mge\config.hpp"

PlayerFishAbilityBehaviour::PlayerFishAbilityBehaviour(World * pWorld, GameObject * pOwner) : _world(pWorld)
{
	Mesh * smallFish = Mesh::load(Config::MGE_MODEL_PATH + "fishLP.obj");
	Texture * texture = Texture::load(Config::MGE_TEXTURE_PATH + "Fish_BaseBLU.png");

	PlayerFishFlock * playerFishFlock = new PlayerFishFlock(pOwner->getWorldPosition(), _world, "playerflock", pOwner);
	playerFishFlock->setMesh(smallFish);
	GPUinstancingMaterial * GPUmat = new  GPUinstancingMaterial(*playerFishFlock->allFish, texture);
	playerFishFlock->setMaterial(GPUmat);
	pOwner->add(playerFishFlock);

	_playerFishFlock = playerFishFlock;

}

void PlayerFishAbilityBehaviour::update(float deltaTime)
{
	if (Input::getKeyDown(sf::Keyboard::Q))
	{
		std::cout << "CALLL THE FIESH" << std::endl;
		_playerFishFlock->CallFish();
	}
}

bool PlayerFishAbilityBehaviour::getIsProtected()
{
	return _playerFishFlock->GetIsProtected();
}

PlayerFishAbilityBehaviour::~PlayerFishAbilityBehaviour()
{
}
