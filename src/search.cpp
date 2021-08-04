int negaMax(int depth) {
  if (depth == 0)
    return evaluate();

  int maxi = INT_MIN;
  for (auto mov : all_moves) {
    int score = -negaMax(depth-1);
    if (score > maxi)
      maxi = score;
  }
  return maxi;
}
