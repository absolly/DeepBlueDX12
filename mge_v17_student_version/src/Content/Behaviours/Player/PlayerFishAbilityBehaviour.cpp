#include "PlayerFishAbilityBehaviour.h"
#include "Content\GameObjects\PlayerFishFlock.hpp"
#include "mge/materials/GPUinstancingMaterial.hpp"

#include "Content\GameObjects\Player.h"
#include "mge\core\SoundManager.hpp"
#include "mge\core\Random.h"
#include "Content\Core\Input.h"
#include "mge\config.hpp"
#include "Content/Hud/Hud.hpp"

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
	_coolDown -= deltaTime;
	if (getIsProtected())
		Hud::getInstance()->setAbilityStatus(0);
	else if(_coolDown <= 0)
		Hud::getInstance()->setAbilityStatus(1);
	else 
		Hud::getInstance()->setAbilityStatus(2);

	if (Input::getKeyDown(sf::Keyboard::Q) && _coolDown <= 0 && Hud::getInstance()->getInventory().hasItem("Relic_tablet.png"))
	{
		std::cout << "CALLL THE FIESH" << std::endl;
		SoundManager::getInstance()->PlayAudio("ability", "ability", false, true, false, 50, "", Random::Range(0.9f, 1.1f));
		Hud::getInstance()->setHintText("");

		_playerFishFlock->CallFish();
		_coolDown = 15;
	}
}

bool PlayerFishAbilityBehaviour::getIsProtected()
{
	return _playerFishFlock->GetIsProtected();
}

PlayerFishAbilityBehaviour::~PlayerFishAbilityBehaviour()
{
}
