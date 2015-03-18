
import os

btjoin = os.path.join

# paths
bt_paths = dict(
	bt_root 		 	= '.',
	bt_cocos 			= btjoin('.', 'external', 'cocos2d-x'),
	bt_cocos_dist 		= btjoin('.', 'external', 'cocos2d-x-dist'),
	bt_lua 				= btjoin('.', 'external', 'BtLua-dist'),
	bt_utouch 			= btjoin('.', 'external', 'utouch'),
	)

# apply these vars to the evironmental variables
for k, v in bt_paths.iteritems():
	os.environ[k] = os.path.abspath(v)
	print '{}={}'.format(k, os.environ[k]) 
