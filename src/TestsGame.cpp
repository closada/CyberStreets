/*#include "tau/tau.h"
#include "Player.hpp"
#include "Enemy.hpp"
#include "AvatarConfig.hpp"
#include "EnemyConfig.hpp"
#include <SFML/Graphics.hpp>

TAU_MAIN()

TEST(Player, InicializacionYStats) {
    sf::Texture dummyTex;
    dummyTex.create(48, 48);

    AvatarConfig config;
    config.damage = 10;
    config.maxHealth = 100;
    config.speed = 5.f;
    config.idleFrames = 4;
    config.idleDuration = 0.1f;
    config.runFrames = 6;
    config.runDuration = 0.1f;
    config.attackFrames = 6;
    config.attackDuration = 0.1f;
    config.runAttackFrames = 6;
    config.runAttackDuration = 0.1f;

    Player p({0.f,0.f}, config, dummyTex, dummyTex, dummyTex, dummyTex);

    CHECK(p.getHealth() == 100);
    CHECK(p.getMaxHealth() == 100);
    CHECK(p.getDamage() == 10);
    CHECK(!p.isDead());
}

TEST(Player, TakeDamageYCooldown) {
    sf::Texture dummyTex;
    dummyTex.create(48,48);

    AvatarConfig config;
    config.damage = 10;
    config.maxHealth = 50;
    config.speed = 5.f;
    config.idleFrames = 4;
    config.idleDuration = 0.1f;
    config.runFrames = 6;
    config.runDuration = 0.1f;
    config.attackFrames = 6;
    config.attackDuration = 0.1f;
    config.runAttackFrames = 6;
    config.runAttackDuration = 0.1f;

    Player p({0.f,0.f}, config, dummyTex, dummyTex, dummyTex, dummyTex);

    // recibe daño
    p.takeDamage(20);
    CHECK(p.getHealth() == 30);

    // no puede recibir daño inmediatamente por cooldown
    p.takeDamage(20);
    CHECK(p.getHealth() == 30); // igual que antes

    // simulamos que pasa el cooldown
    p.update(0.7f);
    p.takeDamage(15);
    CHECK(p.getHealth() == 15);

    p.takeDamage(20); // mata al player
    CHECK(p.getHealth() == 0);
    CHECK(p.isDead());
}

TEST(Enemy, InicializacionYStats) {
    sf::Texture dummyTex;
    dummyTex.create(48,48);

    EnemyConfig config;
    config.maxHealth = 40;
    config.damage = 5;
    config.speed = 3.f;
    config.runFrames = 6;
    config.attackFrames = 4;
    config.deadFrames = 2;
    config.attackCooldown = 1.f;

    Enemy e({0.f,0.f}, config, dummyTex, dummyTex, dummyTex);

    CHECK(e.getHealth() == 40);
    CHECK(e.getMaxHealth() == 40);
    CHECK(e.getDamage() == 5);
    CHECK(!e.isDead());
}

TEST(Enemy, TakeDamage) {
    sf::Texture dummyTex;
    dummyTex.create(48,48);

    EnemyConfig config;
    config.maxHealth = 30;
    config.damage = 5;
    config.speed = 3.f;
    config.runFrames = 6;
    config.attackFrames = 4;
    config.deadFrames = 2;
    config.attackCooldown = 1.f;

    Enemy e({0.f,0.f}, config, dummyTex, dummyTex, dummyTex);

    e.takeDamage(10);
    CHECK(e.getHealth() == 20);

    e.takeDamage(25);
    CHECK(e.getHealth() == 0);
    CHECK(e.isDead());
}

TEST(Player, AtaqueHitDone) {
    sf::Texture dummyTex;
    dummyTex.create(48,48);

    AvatarConfig config;
    config.damage = 10;
    config.maxHealth = 50;
    config.speed = 5.f;
    config.idleFrames = 4;
    config.idleDuration = 0.1f;
    config.runFrames = 6;
    config.runDuration = 0.1f;
    config.attackFrames = 6;
    config.attackDuration = 0.1f;
    config.runAttackFrames = 6;
    config.runAttackDuration = 0.1f;

    Player p({0.f,0.f}, config, dummyTex, dummyTex, dummyTex, dummyTex);

    p.handleAttack();
    CHECK(p.isAttacking());
    CHECK(p.canHit());

    p.markHit();
    CHECK(!p.canHit());
}

TEST(Player, Movimiento) {
    sf::Texture dummyTex;
    dummyTex.create(48,48);

    AvatarConfig config;
    config.damage = 10;
    config.maxHealth = 50;
    config.speed = 5.f;
    config.idleFrames = 4;
    config.idleDuration = 0.1f;
    config.runFrames = 6;
    config.runDuration = 0.1f;
    config.attackFrames = 6;
    config.attackDuration = 0.1f;
    config.runAttackFrames = 6;
    config.runAttackDuration = 0.1f;

    Player p({0.f,0.f}, config, dummyTex, dummyTex, dummyTex, dummyTex);

    sf::Vector2f dir(1.f, 0.f);
    p.handleMovement(dir, 1.f);

    CHECK(p.getPosition().x > 0.f);
    CHECK(p.getCurrentAnimation() == "run");
}

*/
