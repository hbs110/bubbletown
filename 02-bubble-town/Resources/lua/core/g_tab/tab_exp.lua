
require "tab_exp_gen" -- the generated exp table

function get_max_level()    return #t_exp           end
function get_level_exp(lv)  return t_exp[lv].exp    end
