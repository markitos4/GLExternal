        ��  ��                  3      ��� ���     0 	        while (not game:IsLoaded()) do task['wait'](0.5) end

warn('Loaded GLExternal, made by Glosaryyy: [https://discord.gg/xDB7phZtxk]')
<<<<<<< HEAD

local CoreGui = game:GetService('CoreGui')
local RunService = game:GetService('RunService')
local RobloxGui = CoreGui:FindFirstChild('RobloxGui')
local Modules = RobloxGui:FindFirstChild('Modules')

local ScriptsHolder = Instance['new']('ObjectValue', CoreGui)
ScriptsHolder['Name'] = 'GLExternal_Holder'

local ogetfenv = getfenv
local orequire = require
local oxpcall = xpcall
local otable = table
local odebug = debug
local ogame = game
local ocoroutine = coroutine
local ostring = string
=======
warn('Execution and injection times are too long (im aware of this, ill fix it on future updates)')

local CoreGui = game:GetService('CoreGui')
local RunService = game:GetService('RunService')
local Modules = CoreGui:FindFirstChild('RobloxGui')['Modules']

local ScriptsHolder = Instance['new']('ObjectValue', CoreGui)
ScriptsHolder.Name = 'GLExternal_Holder'

local ogetfenv = getfenv
local orequire = require
>>>>>>> 3cdf6157f3ef780faccf71624c225e5ff3f1a93b

local AllModules = {}
local ModulesIndex = 1
local Blacklist = {
	['Common'] = true, ['Settings'] = true, ['PlayerList'] = true, ['InGameMenu'] = true,
	['PublishAssetPrompt'] = true, ['TopBar'] = true, ['InspectAndBuy'] = true,
	['VoiceChat'] = true, ['Chrome'] = true, ['PurchasePrompt'] = true, ['VR'] = true,
	['EmotesMenu'] = true, ['FTUX'] = true, ['TrustAndSafety'] = true
}

for _, Script in ipairs(Modules:GetDescendants()) do
<<<<<<< HEAD
	if Script['ClassName'] == 'ModuleScript' then
		if not Blacklist[Script['Name']] then
=======
	if Script.ClassName == 'ModuleScript' then
		if not Blacklist[Script.Name] then
>>>>>>> 3cdf6157f3ef780faccf71624c225e5ff3f1a93b
			local BlacklistedParent = false
			for BlockedName in pairs(Blacklist) do
				if Script:IsDescendantOf(Modules:FindFirstChild(BlockedName)) then
					BlacklistedParent = true
					break
				end
			end
			if not BlacklistedParent then
				local Clone = Script:Clone()
<<<<<<< HEAD
				Clone['Name'] = 'Arctic'
				table['insert'](AllModules, Clone)
=======
				Clone.Name = 'Arctic'
				table.insert(AllModules, Clone)
>>>>>>> 3cdf6157f3ef780faccf71624c225e5ff3f1a93b
			end
		end
	end
end

local function GetRandomModule()
	ModulesIndex = ModulesIndex + 1

	local FetchedModule = AllModules[ModulesIndex]

<<<<<<< HEAD
	if FetchedModule and FetchedModule['ClassName'] == 'ModuleScript' then
=======
	if FetchedModule and FetchedModule.ClassName == 'ModuleScript' then
>>>>>>> 3cdf6157f3ef780faccf71624c225e5ff3f1a93b
		return FetchedModule
	elseif ModulesIndex < #AllModules then
		return GetRandomModule()
	else
		return nil
	end
end

<<<<<<< HEAD
task['spawn'](function() -- Env 
	ogetfenv(0)['getgenv'] = function()
		return ogetfenv(0)
	end

	ogetfenv(0)['newcclosure'] = function()
		return 'GLExternal', 'BETA OKAY?'
	end

	ogetfenv(0)['isreadonly'] = function(tble)
		return otable['isfrozen'](tble)
	end

	ogetfenv(0)['iscclosure'] = function(closure)
		return odebug['info'](closure, 's') == '[C]'
	end

	ogetfenv(0)['islclosure'] = function(closure)
		return not iscclosure(closure)
	end

	ogetfenv(0)['isexecutorclosure'] = function(closure)
		if iscclosure(closure) then
			return odebug['info'](closure, 'n') == ('' or nil)
		end

		return true
	end

	ogetfenv(0)['getinstances'] = function()
		local instances = {}

		for _, instance in ipairs(ogame:GetDescendants()) do
			otable['insert'](instances, instance)
		end

		return instances
	end

	ogetfenv(0)['getscripts'] = function()
		local scripts = {}

		for _, instance in ipairs(getinstances()) do
			if instance:IsA('LocalScript') or instance:IsA('ModuleScript') then
				otable['insert'](scripts, instance)
			end
		end

		return scripts
	end

	
	getfenv(0)['newcclosure'] = function(closure) 
		if iscclosure(closure) then
			return closure
		end

		local wrapped = ocoroutine['wrap'](function(...)
			local args = {...}
			while true do
				args = { ocoroutine['yield'](closure(unpack(args))) }
			end
		end)

		return wrapped
	end

	getfenv(0)['gethui'] = function()
		return CoreGui
	end --

	getfenv(0)['isscriptable'] = function(object, property)
		local dummy = function(result)
			return result
		end

		local success, result = oxpcall(object['GetPropertyChangedSignal'], dummy, object, property)
		if not success then
			success = not ostring['find'](result, 'scriptable property', nil, true)
		end
		return success
	end

end)

task['wait'](0.50)
task['spawn'](function()
	ScriptsHolder['Value'] = GetRandomModule()

	RunService['RenderStepped']:Connect(function()
		local CurrentScript = ScriptsHolder['Value']
		
		if not CurrentScript then
			ScriptsHolder['Value'] = GetRandomModule()
			CurrentScript = ScriptsHolder['Value']
		end

		local Success, Returned = pcall(function()
			return orequire(CurrentScript)
		end)

		if Success then  
			if type(Returned) == 'table' and Returned['GL-Execution'] and typeof(Returned['GL-Execution']) == 'function' then
=======
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
>>>>>>> 3cdf6157f3ef780faccf71624c225e5ff3f1a93b
				if ModulesIndex == #AllModules then
					ModulesIndex = 1
				end

<<<<<<< HEAD
				CurrentScript:Destroy()
				ScriptsHolder['Value'] = GetRandomModule()
				CurrentScript = ScriptsHolder['Value']

				task['spawn'](setfenv(Returned['GL-Execution'], ogetfenv()))
			end
		end
	end)
end)
=======
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
>>>>>>> 3cdf6157f3ef780faccf71624c225e5ff3f1a93b
 