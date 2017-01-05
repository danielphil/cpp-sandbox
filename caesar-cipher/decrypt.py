from analyse import analyse_text
import json
import collections
import string
import operator

occurrances = {}

# Thanks http://stackoverflow.com/questions/9590382/forcing-python-json-module-to-work-with-ascii
#def ascii_encode_dict(data):
#    ascii_encode = lambda x: x.encode('ascii')
#    return dict(map(ascii_encode, pair) for pair in data.items())

with open('results.json') as json_data:
    #occurrances = json.loads(json_data, object_hook=ascii_encode_dict)
    occurrances = json.load(json_data)

(encrypted_occurrances, character_count) = analyse_text("encrypted.txt")

decryption = collections.OrderedDict()

#sorted_occurances = list(map(lambda t: t[0], sorted(occurrances.items(), key=operator.itemgetter(1), reverse=True)))
#print(sorted_occurances)

for c in string.ascii_lowercase:
    encrypted_occurance = encrypted_occurrances[c];
    distance = {}
    for option in occurrances:
        distance[option] = abs(occurrances[option] - encrypted_occurance)
    
    candidates = list(sorted(distance.items(), key=operator.itemgetter(1)))
    decryption[c] = candidates[0][0]

with open("encrypted.txt") as f:
    for line in f:
        output_line = ''
        for c in line:
            is_upper = c.isupper()
            if c.lower() in decryption:
                if is_upper:
                    output_line += decryption[c.lower()].upper()
                else:
                    output_line += decryption[c]
            else:
                output_line += c
            #print(decryption[c])
            #if c.lower() in decryption:
                #print(c.lower())
            #    output_line += decryption[c.lower()]
            #else:
            #    output_line += c
        print(output_line)