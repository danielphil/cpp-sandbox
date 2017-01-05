import collections
import string
import json
from functools import reduce

def analyse_text(filename):
    occurrences = collections.OrderedDict()
    character_count = 0

    for c in string.ascii_lowercase:
        occurrences[c] = 0

    with open(filename) as textfile:
        for line in textfile:
            characters_only = filter(lambda c: c.isalpha() and c.lower() in occurrences, line)
            lowercase_only = map(lambda c: c.lower(), characters_only)
            for char in lowercase_only:
                occurrences[char] += 1
                character_count += 1

    for char in occurrences.keys():
        occurrences[char] = float(occurrences[char]) / character_count

    return (occurrences, character_count)

if __name__ == "__main__":
    results = [analyse_text("1342-0.txt"), analyse_text("11-0.txt"), analyse_text("pg1661.txt")]
    #results = [analyse_text("1342-0.txt")]
    total_characters = sum(map(lambda t: t[1], results))

    final_result = collections.OrderedDict()
    for c in string.ascii_lowercase:
            final_result[c] = 0

    for result in results:
        for pair in result[0].items():
            final_result[pair[0]] += pair[1] * (result[1] / total_characters)

    with open('results.json', 'w') as outfile:
        json.dump(final_result, outfile, sort_keys=True, indent=4, separators=(',', ': '))
