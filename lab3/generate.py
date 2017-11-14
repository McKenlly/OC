import random
import string
import copy
from random import choice
MAX_VALUE_LEN = 1000

SIZE_ARRAY = 5000
def generate_random_value():
    return "".join([random.choice(string.ascii_lowercase) for _ in range(1, MAX_VALUE_LEN)])


if __name__ == "__main__":
    for num in range(1, 10):
        values = list()
        output_filename = "tests/{:02d}.t".format(num)
        with open(output_filename, 'w') as output:
            numtest = random.randint(4000, SIZE_ARRAY)
            output.write("{}\n".format(numtest))
            for _ in range(0, numtest):
                value = random.randint( 1, MAX_VALUE_LEN )
                values.append(value)
                output.write("{}\t".format(value))
        # Answer.
        # values[0][0] -- key
        # values[0][1] -- value
        # values[0][2] -- keyString
        output_filename = "tests/{:02d}.txt".format(num)
        with open(output_filename, 'w') as output:
            values = sorted(values)
            for value in values:
                #print(value[0])
                output.write("{}\t".format(value))
            output.write("\n")
