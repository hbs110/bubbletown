
import os

btjoin = os.path.join

# paths
bt_paths = dict(
	bt_root 		= '.',
	bt_cocos2dx 	= btjoin('.', 'external', 'cocos2d-x'),
	bt_lua 			= btjoin('.', 'external', 'BtLua-dist'),
	bt_utouch 		= btjoin('.', 'external', 'utouch'),
	)

# apply these vars to the evironmental variables
for k, v in bt_paths.iteritems():
	os.environ[k] = os.path.abspath(v)
	print '{}={}'.format(k, os.environ[k]) 
