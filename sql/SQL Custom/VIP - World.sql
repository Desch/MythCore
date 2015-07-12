DELETE FROM command WHERE name IN ("vip", "vip bank", "vip save", "vip tele", "vip tele dalaran", "vip tele zoneshop", "vip tele shattrath", 
"vip tele guruarena", "vip tele stormwind", "vip tele ironforge", "vip tele darnassus", "vip tele exodar", "vip tele orgrimmar",
"vip tele thunderbluff", "vip tele undercity", "vip tele silvermoon", "vip gamble", "vip roulette");
INSERT INTO command (name, security, help) VALUES
("vip", 1, "Syntax: .vip

VIP commands."),
("vip bank", 1, "Syntax: .vip bank

Show your bank inventory."), 
("vip save", 1, "Syntax: .vip save

Saves your character."), 
("vip tele", 1, "Syntax: .vip tele

Teleports you to entered location."), 
("vip tele dalaran", 1, "Syntax: .vip tele dalaran

Teleports you to entered location."), 
("vip tele zoneshop", 1, "Syntax: .vip tele zoneshop

Teleports you to Dalaran"), 
("vip tele shattrath", 1, "Syntax: .vip tele shattrath

Teleports you to Shattrath"), 
("vip tele guruarena", 1, "Syntax: .vip tele guruarena

Teleports you to Gurubashi Arena"), 
("vip tele stormwind", 1, "Syntax: .vip tele stormwind

Teleports you to Stormwind(only for Alliance)"), 
("vip tele ironforge", 1, "Syntax: .vip tele ironforge

Teleports you to Ironforge(only for Alliance)"), 
("vip tele darnassus", 1, "Syntax: .vip tele darnassus

Teleports you to Darnassus(only for Alliance)"), 
("vip tele exodar", 1, "Syntax: .vip tele exodar

Teleports you to Exodar(only for Alliance)"), 
("vip tele orgrimmar", 1, "Syntax: .vip tele orgrimmar

Teleports you to Orgrimmar(only for Horde)"),
("vip tele thunderbluff", 1, "Syntax: .vip tele thunderbluff

Teleports you to Thunder Bluff(only for Horde)"), 
("vip tele undercity", 1, "Syntax: .vip tele undercity

Teleports you to Undercity(only for Horde)"), 
("vip tele silvermoon", 1, "Syntax: .vip tele silvermoon

Teleports you to Silvermoon(only for Horde)");  
