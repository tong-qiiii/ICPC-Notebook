"""Period, overlap, palindrome and forbidden-substring guide derivations."""
from itertools import product
import random


def run():
    rg = random.Random(170008)
    for _ in range(500):
        s = ''.join(rg.choice('abc') for _ in range(rg.randrange(1, 14)))
        pi = [0]*len(s)
        for i in range(1, len(s)):
            j = pi[i-1]
            while j and s[i] != s[j]:
                j = pi[j-1]
            pi[i] = j+int(s[i] == s[j])
        border, j = [], pi[-1]
        while j:
            border.append(j)
            j = pi[j-1]
        assert sorted(border) == [k for k in range(1, len(s)) if s[:k] == s[-k:]]
        p = len(s)-pi[-1]
        complete = p if len(s) % p == 0 else len(s)
        assert complete == min(k for k in range(1, len(s)+1)
                               if len(s) % k == 0 and s[:k]*(len(s)//k) == s)
        periods = [k for k in range(1, len(s)+1) if s[k:] == s[:len(s)-k]]
        assert periods == [k for k in range(1, len(s)+1)
                           if all(s[i] == s[i % k] for i in range(len(s)))]
        odd, even = [], []
        for i in range(len(s)):
            r = 1
            while i-r >= 0 and i+r < len(s) and s[i-r] == s[i+r]:
                r += 1
            odd.append(r)
            r = 0
            while i-r-1 >= 0 and i+r < len(s) and s[i-r-1] == s[i+r]:
                r += 1
            even.append(r)
        palindromes = [s[i:j] for i in range(len(s)) for j in range(i+1, len(s)+1)
                       if s[i:j] == s[i:j][::-1]]
        assert sum(odd)+sum(even) == len(palindromes)
        assert max([2*x-1 for x in odd]+[2*x for x in even]) == max(map(len, palindromes))
        a, b = s, ''.join(rg.choice('abc') for _ in range(rg.randrange(1, 9)))
        overlap = max(k for k in range(min(len(a), len(b))+1) if a[len(a)-k:] == b[:k])
        joined = a+b[overlap:]
        assert joined.startswith(a) and joined.endswith(b)
        assert len(joined) == min(len(a)+len(b)-k for k in range(min(len(a), len(b))+1)
                                  if a[len(a)-k:] == b[:k])
        # LCP truncation for comparing finite substrings rather than suffixes.
        i, j = rg.randrange(len(s)), rg.randrange(len(s))
        u, v = rg.randrange(len(s)-i+1), rg.randrange(len(s)-j+1)
        h = 0
        while i+h < len(s) and j+h < len(s) and s[i+h] == s[j+h]:
            h += 1
        h = min(h, u, v)
        comparison = (u > v)-(u < v) if h == min(u, v) else (s[i+h] > s[j+h])-(s[i+h] < s[j+h])
        assert comparison == (s[i:i+u] > s[j:j+v])-(s[i:i+u] < s[j:j+v])
    for _ in range(180):
        patterns = set(''.join(rg.choice('ab') for _ in range(rg.randrange(1, 4)))
                       for _ in range(rg.randrange(1, 5)))
        # Build safe prefix automaton independently from the existing AC implementation.
        prefixes = {''}
        for s in patterns:
            prefixes.update(s[:i] for i in range(1, len(s)))
        prefixes = {s for s in prefixes if not any(p in s for p in patterns)}
        states = {s: 0 for s in prefixes}
        states[''] = 1
        for n in range(7):
            want = sum(not any(p in ''.join(t) for p in patterns) for t in product('ab', repeat=n))
            assert sum(states.values()) == want
            nxt = {s: 0 for s in prefixes}
            for s, ways in states.items():
                for c in 'ab':
                    t = s+c
                    if any(t.endswith(p) for p in patterns):
                        continue
                    target = max((p for p in prefixes if t.endswith(p)), key=len)
                    nxt[target] += ways
            states = nxt
    print('PASS string models: borders/periods, overlaps, palindrome counts, LCP truncation, forbidden-state DP', flush=True)
