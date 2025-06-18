while (not game:IsLoaded()) do task['wait'](0.5) end

warn('Loaded GLExternal, made by Glosaryyy: [https://discord.gg/xDB7phZtxk]')
warn('Execution and injection times are too long (im aware of this, ill fix it on future updates)') 

local CoreGui = game:GetService('CoreGui')
local RunService = game:GetService('RunService')
local Modules = CoreGui:FindFirstChild('RobloxGui')['Modules']

local ScriptsHolder = Instance['new']('ObjectValue', CoreGui)
ScriptsHolder.Name = 'GLExternal_Holder'

local ogetfenv = getfenv
local orequire = require

local AllModules = {}
local ModulesIndex = 1
local Blacklist = {
	['Common'] = true, ['Settings'] = true, ['PlayerList'] = true, ['InGameMenu'] = true,
	['PublishAssetPrompt'] = true, ['TopBar'] = true, ['InspectAndBuy'] = true,
	['VoiceChat'] = true, ['Chrome'] = true, ['PurchasePrompt'] = true, ['VR'] = true,
	['EmotesMenu'] = true, ['FTUX'] = true, ['TrustAndSafety'] = true
}

for _, Script in ipairs(Modules:GetDescendants()) do
	if Script.ClassName == 'ModuleScript' then
		if not Blacklist[Script.Name] then
			local BlacklistedParent = false
			for BlockedName in pairs(Blacklist) do
				if Script:IsDescendantOf(Modules:FindFirstChild(BlockedName)) then
					BlacklistedParent = true
					break
				end
			end
			if not BlacklistedParent then
				local Clone = Script:Clone()
				Clone.Name = 'Arctic'
				table.insert(AllModules, Clone)
			end
		end
	end
end

local function GetRandomModule()
	ModulesIndex = ModulesIndex + 1

	local FetchedModule = AllModules[ModulesIndex]

	if FetchedModule and FetchedModule.ClassName == 'ModuleScript' then
		return FetchedModule
	elseif ModulesIndex < #AllModules then
		return GetRandomModule()
	else
		return nil
	end
end

task.wait(1)
task.spawn(function()
	ScriptsHolder.Value = GetRandomModule()

	while task.wait(0.5) do
		local CurrentScript = ScriptsHolder.Value
		
		if not CurrentScript then
			ScriptsHolder.Value = GetRandomModule()
			CurrentScript = ScriptsHolder.Value
		end

		local Returned = nil
		local Success = pcall(function()
			Returned = orequire(ScriptsHolder.Value)
		end)


		if Success then 
			if type(Returned) == 'table' and Returned['Arctic'] and typeof(Returned['Arctic']) == 'function' then
				if ModulesIndex == #AllModules then
					ModulesIndex = 1
				end

				task.spawn(setfenv(Returned['Arctic'], setmetatable({}, {__index = ogetfenv()})))

				CurrentScript:Destroy()
				ScriptsHolder.Value = GetRandomModule()
			else
				ScriptsHolder.Value = GetRandomModule()
				CurrentScript = ScriptsHolder.Value
			end
		end
	end
end)