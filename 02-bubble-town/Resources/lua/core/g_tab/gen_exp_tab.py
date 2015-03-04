

ExpFile = "tab_exp_gen.lua"
MaxLevel = 100
TabName = "t_exp"


def std_exp_formula(lv):
    return (lv + 1) * (lv + 1) * 10


def format_line(lv, exp, total):
    return "{}[{}] = {{ exp = {}, total = {} }}".format(TabName, lv, exp, total)


def format_section():
    total = 0
    lines = []
    for x in xrange(1, MaxLevel):
        exp = std_exp_formula(x)
        total += exp
        lines.append(format_line(x, exp, total))
    return "\n".join(lines)

def main():
    with open(ExpFile, "w") as f:
        f.write("\n")
        f.write(TabName + " = {}\n")
        f.write(format_section())

if __name__ == '__main__':
    main()


