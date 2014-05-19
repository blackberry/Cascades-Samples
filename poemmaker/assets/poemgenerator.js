// Java script function used to generate different parts of the poem.
// This functions generates a random poem line. A poem is built up by concatenating words
// according to the following scheme.
// line1: <adjective> + <noun> + 
// line2: <verb> + <adverb> +
// line3: <preposition> + <noun>.
// For example: "A seagull waits slowly over an elephant"
function generatePoemLine (line) {
	line = parseInt (line);
	var newPoem = null;
	var randomWord = 0;
		
    // Poetic words
	var adjectives = new Array ("An anxious", "A beautiful", "A cool", "A delightful", "An energetic", "A fluffy", "A gentle", "A helpful", "A hilarious", "An immense", "A jolly", "A lovely", "A massive", "A nosy", "An orange", "A perfect", "A quaint", "A round", "A short", "A sweet", "A tasty", "An upset", "A weary", "A yellow", "A green", "A red", "A blue", "A green");
	var nouns = new Array ("seagull", "squid", "mouse", "train", "blackberry", "strawberry", "planet", "cloud", "rose", "star", "lake", "bat", "banana", "chicken", "donkey", "fireman", "goldfish", "holiday", "jellyfish", "kitten", "ghost", "lake", "mother", "pancake", "queen", "stranger", "toothbrush", "volcano", "zebra");
	var verbs = new Array ("eats", "walks", "grows", "hunts", "roars", "waits", "swims", "revolves", "remembers", "quivers", "offends", "runs", "anticipates", "broadcasts", "concludes", "demonstrates", "encourages", "fights", "guards", "hides", "investigates", "jumps", "kicks", "loves", "notes", "observes", "promotes", "refuses", "tickles", "wanders");
	var adverbs = new Array ("slowly", "magnificently", "jealously", "beautifully", "dangerously", "quietly", "resentfully", "approvingly", "bleakly", "cursorily", "daftly", "enthusiastically", "flirtatiously", "gracefully", "haphazardly", "implicitly", "judgementally", "lethargically", "peacefully", "respectfully", "suspiciously", "thickheadedly", "unmusically", "violently");//"quintessentially"
	var prepositions = new Array ("aboard", "against", "after", "among", "around", "at", "behind", "beside", "beyond", "despite", "following", "from", "inside", "near", "past", "through", "on", "of", "over", "before", "across", "in", "like", "under", "towards", "above", "onto", "opposite", "outside", "over", "below");
		
    // A random line of a poem is generated
	if (line == 1) {
        //<adjective> + <noun>
		randomWord = (Math.random () * (adjectives.length - 1));
		newPoem = adjectives[parseInt (randomWord)] + " ";
		randomWord = (Math.random () * (nouns.length - 1));
		newPoem = newPoem + nouns[parseInt (randomWord)];
	} else if (line == 2) {
        //<verb> + <adverb>
		randomWord = (Math.random () * (verbs.length - 1));
		newPoem = verbs[parseInt (randomWord)] + " ";
		randomWord = (Math.random () * (adverbs.length - 1));
		newPoem = newPoem + adverbs[parseInt (randomWord)];
	} else if (line == 3) {
        //<preposition> + <noun>
		randomWord = (Math.random () * (prepositions.length - 1));
		newPoem = prepositions[parseInt (randomWord)] + " ";
		randomWord = (Math.random () * (nouns.length - 1));
		newPoem = newPoem + "a " + nouns[parseInt (randomWord)] + ".";
	}
	return newPoem;
}