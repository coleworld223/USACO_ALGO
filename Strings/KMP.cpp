// Algorithm : Prefix Function
// Read Source : CP ALGO (https://cp-algorithms.com/string/prefix-function.html)
// Implementation : Own
// Tested : No

/*
  prefixFunction : P
  P[i] = longest proper suffix ending at i, that is also a prefix in string (say s)

  KMP {
    P; // s -> string , t -> text
    search(string s, string t); (find count of occurences of s in t / searching substrings)
    countPrefix(string s, string t); (find the count of occurences of all prefixes of s in t)
    countSubstr(string s); (count number of different substrings in s)
    compressStr(string s); (find the smallest string (say cmp) such that s = (cmp * timesOfOccr))
    buildAutomaton(string s); (build aut[currentPrefixSuffixMaxLength][appendChar] = newPrefixSuffixMaxLenght)
  }

  Let's implement
  build automation for s and utilise it for different functions

*/

class KMP {
private:
  string s;
  vector <vector <int>> aut;
public:
  vector <int> prefixFunction(string& s) { // O(n)
    int n = s.size();
    vector <int> P(n);
    for (int i = 1; i < n; ++i) {
      int j = P[i - 1];
      while (j > 0 and s[j] != s[i]) { j = P[j - 1]; }
      if (s[i] == s[j]) { j++; }
      P[i] = j;
    }
    return P;
  }
  void buildAutomaton(string _s) { // O(26n)
    s = _s;
    s += "#";
    int n = s.size();
    aut.resize(n, vector <int>(26));
    vector <int> P = prefixFunction(s);
    for (int i = 0; i < n; ++i) {
      for (int c = 0; c < 26; c++) {
        if (i > 0 and c + 'a' != s[i]) { aut[i][c] = aut[P[i - 1]][c]; }
        else { aut[i][c] = i + (c + 'a' == s[i]); }
      }
    }
  }
  int countSubstr(string s, string t) {
    //find count of s in t
    int n = s.size();
    int m = t.size();
    string X = s + "#" + t;
    int cnt = 0;
    vector <int> P = prefixFunction(X);
    for (int i = n + 1; i < n + m + 1; ++i) {
      if (P[i] == n)
        cnt++;
    }
    return cnt;
  }
  long long countPrefix(string s, string t) {
    //find count of all prefix of s in t
    int n = s.size();
    int m = t.size();
    string X = s + "#" + t;
    vector <int> P = prefixFunction(X);
    vector <int> dp(n + m + 1);
    long long ans = 0;
    for (int i = 0; i < n + m + 1; ++i) {
      dp[i] = (P[i] != 0) + (P[i] > 0 ? dp[P[P[i] - 1]] : 0);
      ans += dp[i];
    }
    return ans;
  }
  int countExtraSubstrAdded(string s, char c) {
    //add c to end of s find the the count of new(unique) substrings added
    s.push_back(c);
    reverse(s.begin(), s.end());
    vector <int> P = prefixFunction(s);
    int n = s.size();
    if (n == 1) { return 1; }
    int ans = 0;
    int mx = 0;
    for (int i = 1; i < n; ++i) {
      mx = max(mx, P[i]);
    }
    return n - mx;
  }
  long long countUniqueSubstr(string t) {
    long long ans = 0;
    string s = "";
    for (int i = 0; i < t.size(); ++i) {
      ans += countExtraSubstrAdded(s, t[i]);
      s.push_back(t[i]);
    }
    return ans;
  }
  string compressed(string s) {
    //find the smallest string say cmp such that cmp * times = s
    vector <int> P = prefixFunction(s);
    int n = s.size();
    int k = n - P[n - 1];
    if (n % k == 0) { return s.substr(0, k); }
    return s;
  }
};