# UE5 — Effect System Plugin

Plugin Unreal Engine 5 pour la gestion d'effets sur les characters.

## Features
- Effets instantanés, Buffs / Debuffs
- Durée par temps (float) ou par tours (int)
- Stacking configurable
- Tick centralisé via GameInstanceSubsystem
- Blueprint-friendly

## Installation
1. Copie le dossier `EffectSystem/` dans `TonProjet/Plugins/`
2. Clic droit sur `.uproject` → Generate Visual Studio project files
3. Compile et active le plugin dans Edit → Plugins

## Dépendances
- Unreal Engine 5.x
- Ton StatComponent doit implémenter `IStatable`
