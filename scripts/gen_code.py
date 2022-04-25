import os

patterns = []
patterns.append("0111010011100011100001100")
patterns.append("011011101110110101011000110000101000")
patterns.append("011111101000110111101010101101101010")
patterns.append("011110101101110000110000100001010010")
patterns.append("0111111101111111011101110100111100011100001100000")
patterns.append("0111111101111111011001110100111100011000001100000")
patterns.append("0111101111011110")
patterns.append("0111110111110111110111110")

pattern_sizes = [5, 6, 6, 6, 7, 7, 4, 5]
graphs = []
graphs.append("wiki-vote.g")
graphs.append("patents.g")
graphs.append("mico.g")
graphs.append("livejournal.g")
graphs.append("orkut.g")
g = []
g.append('wv')
g.append("pt")
g.append("mc")
g.append("lj")
g.append("or")

for g_idx, graph in enumerate(graphs):
    for p in range(len(patterns)):
        #log_name = graph + "_" + "p" + str(p + 1) + ".log"
        log_name = "../auto/" + g[g_idx] + "_p" + str(p + 1) + "_inject.cu" 
        os.system("./bin/final_generator ~/data/" + graph + " " + str(pattern_sizes[p]) + " " + str(patterns[p]) + " > " + log_name)

os.system("cd ../auto; bash concat.sh")