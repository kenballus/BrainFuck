import sys

ARRAY_SIZE = 30000

def main():
    if len(sys.argv) != 2:
        print("USAGE: python3 bf.py filename.bf")
        return

    ifile = open(sys.argv[1], "r")
    ifile = ifile.read()

    array = [0] * ARRAY_SIZE
    index = 0

    loop_locs = []
    wrote = False

    char_index = 0
    while char_index < len(ifile):
        char = ifile[char_index] # The input character we're currently looking at

        # Increment value
        if char == '+':
            array[index] += 1

        # Decrement value
        elif char == '-':
            array[index] -= 1

        # Increment pointer
        elif char == '>':
            index += 1
            if index >= ARRAY_SIZE:
                print("ERROR: Indexed off the right end of the tape")
                return

        # Decrement pointer
        elif char == '<':
            index -= 1
            if index < 0:
                print("ERROR: Indexed off the left end of the tape")
                return

        # Output
        elif char == '.':
            if array[index] <= 0x10FFFF: # Limit of chr()
                print(chr(array[index]), end="")
                wrote = True

        # Input
        elif char == ',':
            i = input()
            if len(i) > 0:
                array[index] = ord(i[0])

        # Start loop
        elif char == '[':
            if array[index] == 0:
                counter = 1
                while counter:
                    char_index += 1
                    if char_index >= len(ifile):
                        print('ERROR: "[" without corresponding "]"')
                        return

                    elif ifile[char_index] == '[':
                        counter += 1
                    elif ifile[char_index] == ']':
                        counter -= 1
            else:
                loop_locs.append(char_index)

        # End loop
        elif char == ']':
            if array[index] != 0:
                if len(loop_locs) == 0:
                    print('ERROR: "]" without corresponding "["')
                    return

                char_index = loop_locs[-1]
            else:
                loop_locs.pop()

        char_index += 1

    if wrote:
        print()

    if index <= 10:
        start = 0
        end = index + 11
        mode = 0
    elif index < ARRAY_SIZE - 10:
        start = index - 10
        end = index + 11
        mode = 1
    else:
        start = index - 10
        end = ARRAY_SIZE
        mode = 2

    print("=> ", end="")

    if mode != 0:
        print("... ", end="")

    for i in range(start, end):
        if i == index:
            print("["+str(array[i])+"] ", end="")
        else:
            print(array[i], end=" ")

    if mode != 2:
        print("...", end="")

    print()

main()