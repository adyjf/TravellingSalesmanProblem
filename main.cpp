#include <fstream>
#include <string>
#include <ctime>

#include "utilities.cpp"

using namespace lemon;

/*
Fontion principale: lecture du fichier LGF et Branch&Bound
*/
int main(int argc, char* argv[])
{
  float temps;
  clock_t t1, t2;
  std::string file_name;
  int CommandLine = parseCommandLine(&argc, argv, &file_name);

  if(CommandLine == 1){
    // Initialisation graphe et lecture depuis le fichier .lgf
    SmartDigraph digraph;
    SmartDigraph::ArcMap<int> weight(digraph);
    
    try{
      std::cout << "Reading graph from " << file_name << "..." << std::endl;
      digraphReader(digraph, file_name).
      arcMap("capacity", weight).
      node("source", source).
      node("target", target).
      run();
    } catch(Exception& error){
      std::cerr << "Error: " << error.what() << std::endl;
      return -1;
    }
    std::cout << std::endl;

    t1 = clock();

    // Branch and Bound
    SmartDigraph::ArcMap<bool> mst(digraph);
    //UPPER_BOUND = 2*kruskal(digraph, weight, mst);
    SmartDigraph::NodeMap<bool> node_map(digraph, true);
    SmartDigraph::ArcMap<bool> path(digraph, true);
    for(SmartDigraph::ArcIt i(digraph); i!=INVALID; ++i){
      if(weight[i]>=9999)
        path[i] = false;
    }
    SmartDigraph::ArcMap<bool> minspantree_arc_map(digraph, false);
    SmartDigraph::ArcMap<bool> best_path(digraph, false);
    
    std::cout << "I'm working..." << std::endl;
    BB(&digraph, &source, &node_map, &path, &minspantree_arc_map, &best_path, &weight);
    
    t2 = clock();
    temps = (float)(t2-t1)/CLOCKS_PER_SEC;

    // Display results
    /*std::ofstream fichier("resultat.txt", std::ios::app);
    if (fichier){
      fichier << "==============================" <<std::endl;
      fichier << "Temps d'exécution: " << temps << " secondes" << std::endl;
      fichier << "Result: UB = " << UPPER_BOUND << std::endl << std::endl;
      for(SmartDigraph::ArcIt i(digraph); i!=INVALID; ++i)
        fichier << digraph.id(i) << "\t(" << digraph.id(digraph.source(i))+1 << "," << digraph.id(digraph.target(i))+1 << ")\t" << best_path[i] << "\t" << weight[i] <<std::endl;
      fichier << "==============================" <<std::endl;
      fichier.close();
    }*/
    std::cout << "==============================" <<std::endl;
    std::cout << "Temps d'exécution: " << temps << " secondes" << std::endl;
    std::cout << "Result: UB = " << UPPER_BOUND << std::endl << std::endl;
    for(SmartDigraph::ArcIt i(digraph); i!=INVALID; ++i)
      std::cout << digraph.id(i) << "\t(" << digraph.id(digraph.source(i))+1 << "," << digraph.id(digraph.target(i))+1 << ")\t" << best_path[i] << "\t" << weight[i] <<std::endl;
    std::cout << "==============================" <<std::endl;

    // Generate dot file
    if(image){
      std::ofstream fichier("graph.dot", std::ios::out);
      if(fichier){
        fichier << "digraph G {" << std::endl;
        fichier << "\tnode [shape=circle]" << std::endl;
        fichier << "\tedge [arrowhead=open]" << std::endl;
	
        for(SmartDigraph::ArcIt i(digraph); i!=INVALID; ++i){
          if(weight[i]<9999){
            fichier << "\t" << digraph.id(digraph.source(i))+1 << "->" << digraph.id(digraph.target(i))+1;
            if(best_path[i])
              fichier << " [color=red, arrowsize=2]";
            else
              fichier << " [arrowsize=0.5]";
            fichier << ";" << std::endl;
          }
        }

        fichier << "}" << std::endl;
        fichier.close();
      }
      else
        std::cout << "Couldn't create .dot" << std::endl;
    }
  }
  
  return 0;
}