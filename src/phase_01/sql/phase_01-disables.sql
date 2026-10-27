DELETE FROM `disables` WHERE `sourceType` = 1 AND `entry` IN (
    4903, -- Warlord's Command
    7028, -- Twisted Evils
    7029, -- Vyletongue Corruption
    7041, -- Vyletongue Corruption
    7064, -- Corruption of Earth and Seed
    7065, -- Corruption of Earth and Seed
    7067, -- The Pariah's Instructions
    7068, -- Shadowshard Fragments
    7070, -- Shadowshard Fragments
    8181 -- Confront Yeh'kinya
);
DELETE FROM `disables` WHERE `sourceType` = 2 AND `entry` IN (
    249, -- Onyxia's Lair
    309, -- Zul'Gurub
    349, -- Maraudon
    409 -- Molten Core
);
