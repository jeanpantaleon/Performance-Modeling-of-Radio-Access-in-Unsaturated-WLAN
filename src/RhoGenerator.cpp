#include "RhoGenerator.hpp"
#include "utils.hpp"

#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <fstream>


using namespace std;

vector<double> generateUniformRhos(int n, double inf, double sup)
{
  vector<double> rhos(n);
  for (int i = 0; i < n; i++)
  {
    rhos[i] = getRandom();
    while(rhos[i] < inf || rhos[i] > sup) rhos[i] = getRandom();
  }
  return rhos;
}

vector<double> generateGaussianRhos(int n, double mu, double sigma)
{
  vector<double> rhos;
  for (int i = 0; i < n; i++)
  {
    double rho = getRandomGaussian(mu, sigma);
    while (rho < 0 || rho > 1) rho = getRandomGaussian(mu, sigma);
    rhos.push_back(rho);
  }
  return rhos;
}

vector<double> generateDoubleGaussianRhos(int n, double mu1, double sigma1, double mu2, double sigma2, double ratio)
{
  int n1 = round(n*ratio);
  int n2 = n - n1;
  vector<double> rhos = generateGaussianRhos(n1, mu1, sigma1);
  vector<double> rhos2 = generateGaussianRhos(n2, mu2, sigma2);
  rhos.insert(rhos.end(), rhos2.begin(), rhos2.end());
  random_shuffle(rhos.begin(), rhos.end());
  return rhos;
}

vector<double> generateSpatialGaussianRhos(const Graph& G, int parts, const vector<double>& params)
{
  vector<double> rhos(G.getNumVertices());
  for (int v = 0; v < G.getNumVertices(); v++)
  {
    int row = fmod(G.getPosition(v)[0], 1.0/parts);
    int col = fmod(G.getPosition(v)[1], 1.0/parts);
    int id = row + parts*col;
    double rho = getRandomGaussian(params[2*id], params[2*+1]);
    while (rho < 0 || rho > 1) rho = getRandomGaussian(params[2*id], params[2*id+1]);
    rhos[v] = rho;
  }
  return rhos;
}

std::vector<double> fromRhoFile(const std::string filename, int nb_nodes)
{
  std::ifstream rhoFile(filename);
  std::string line;

  std::vector<double> rhos(nb_nodes, 0);

  while (getline(rhoFile, line))
  {
    // tokenize line
    std::vector<std::string> tokens = tokenize(line, " ");
    rhos[std::stoi(tokens[0])] = std::stod(tokens[1]);
  }
  return rhos;
}
