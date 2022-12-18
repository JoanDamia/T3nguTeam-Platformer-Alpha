//#ifndef __ENEMIES_H__
//#define __ENEMIES_H__
//
//#include "Module.h"
//#include "Physics.h"
//#include "Animation.h"
//#include "DynArray.h"
//
//enum class ENEMY_TYPE
//{
//	GROUND,
//	AIR
//};
//
//enum class ENEMY_STATE
//{
//	IDLE,
//	HUNT,
//	RETURN,
//	DEATH
//};
//
//struct Enemy {
//	b2Body* body;
//
//	float origin_x, origin_y;
//	float x, y;
//	int w = 16, h = 16;
//	float speed;
//
//	ENEMY_TYPE type;
//
//	Animation* currentAnimation = NULL;
//
//	bool lookLeft;
//
//	float detectionRange;
//	bool enemy_spoted;
//
//	ENEMY_STATE state;
//
//	// only used in ground enemies
//	float idleOb_x;
//	bool obLeft;
//
//	// only used in air enemies
//	int cd_air_enemy = 0;
//
//	bool plan_to_delete = false;
//
//	PathFinding* path_save;
//};
//
//class Enemies : public Module
//{
//public:
//	Enemies();
//
//	virtual ~Enemies();
//
//	bool Awake();
//
//	bool Start();
//
//	bool PreUpdate();
//
//	bool Update(float dt);
//
//	bool PostUpdate();
//
//	bool CleanUp();
//
//	bool Load(pugi::xml_node&);
//	bool Save(pugi::xml_node&);
//
//	SDL_Texture* pig = NULL;
//	SDL_Texture* bat = NULL;
//
//	DynArray<Enemy> enemies;
//
//	void CreateGroundEnemy(float x, float y);
//	void CreateAirEnemy(float x, float y);
//
//	void MoveGroundEnemy(Enemy* enemy, float dt);
//	void MoveAirEnemy(Enemy* enemy, float dt);
//	void CheckAirEnemy(Enemy* enemy, float dt);
//	void CheckPlayer(Enemy* enemy);
//
//	void EnemyHunting(Enemy* enemy, float dt);
//	void EnemyReturning(Enemy* enemy, float dt);
//
//	void KillEnemy(float x, float y);
//
//	Animation currentPigAnimation;
//	Animation idlePigRightAnimation;
//	Animation idlePigLeftAnimation;
//	Animation goPigRightAnimation;
//	Animation goPigLeftAnimation;
//	Animation jumpPigRightAnimation;
//	Animation jumpPigLeftAnimation;
//	Animation fallPigRightAnimation;
//	Animation fallPigLeftAnimation;
//	Animation PigDeathLAnim;
//	Animation PigDeathRAnim;
//
//	Animation currentBatAnimation;
//	Animation idleBatRightAnimation;
//	Animation idleBatLeftAnimation;
//	Animation goBatRightAnimation;
//	Animation goBatLeftAnimation;
//	Animation jumpBatRightAnimation;
//	Animation jumpBatLeftAnimation;
//	Animation fallBatRightAnimation;
//	Animation fallBatLeftAnimation;
//	Animation BatDeathLAnim;
//	Animation BatDeathRAnim;
//};
//
//#endif
