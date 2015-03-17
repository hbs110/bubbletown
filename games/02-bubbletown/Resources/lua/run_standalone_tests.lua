
package.path = package.path..';lua/?.lua'
package.path = package.path..require "packages"

-- for tests only
package.path = package.path..';lua/tests/?.lua'

local ts_player = require "ts_player"

ts_player.run() 
