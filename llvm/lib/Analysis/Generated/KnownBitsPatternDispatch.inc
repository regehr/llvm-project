#define DEBUG_TYPE "knownbits-pattern-dispatch-stub"

static void computePatternKBMatches(const Operator *, const SimplifyQuery &,
                                    unsigned,
                                    SmallVectorImpl<PatternMatchKB> &) {}

static void recordPatternImpact(unsigned, unsigned, bool) {}

static void recordPatternHistogram(unsigned, const SmallVectorImpl<KnownBits> &,
                                   unsigned, bool) {}
