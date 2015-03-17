require "g_util"

archiving = {}

local _procs = {}
local _activeProfileName = DEFAULT_PLAYER_PROFILE


function archiving.register(sliceName, savingProc, loadingProc)
	_procs[sliceName] = { save=savingProc, load=loadingProc }
end


function archiving.savefile()
	if _activeProfileName == DEFAULT_PLAYER_PROFILE then
		-- print("profile not been correctly initialized, saving request ignored.")
		-- return true
		_activeProfileName = TEST_PLAYER_PROFILE
	end

	local function _save()
		local chunk = {}
		for k,v in pairs(_procs) do
			if v == nil or v.save == nil then
				return nil
			end

			local slice = v.save()
			if slice == nil then
				return nil
			end

			chunk[k] = slice
		end

		-- g_print_table_pretty(chunk)
		return chunk
	end

	local chunk = _save()
	if chunk == nil then
		return false
	end

	local profilePath = g_build_profile_path(_activeProfileName)
	if not g_save_json(profilePath, chunk) then
		return false
	end

 	g_checkpoint(string.format("player profile ('%s') saved.", _activeProfileName))
	return true
end	


function archiving.loadfile(profileName)
	local function _load(chunk)
		for k,v in pairs(_procs) do
			if v == nil or v.load == nil then
				return false
			end

			if chunk[k] ~= nil then
				if not v.load(chunk[k]) then
					return false
				end
			end
		end
		return true
	end

	local chunk = g_load_json(g_build_profile_path(profileName))
	if chunk == nil then 
		return false
	end
	
	if not _load(chunk) then
		return false
	end

	_activeProfileName = profileName
 	g_checkpoint(string.format("player profile ('%s') loaded.", _activeProfileName))
	return true
end

return archiving
