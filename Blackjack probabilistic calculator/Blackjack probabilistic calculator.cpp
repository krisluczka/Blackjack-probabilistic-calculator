#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <random>
#include <vector>

using namespace std;

enum cards {
    error_card = 1,
    card_2,
    card_3,
    card_4,
    card_5,
    card_6,
    card_7,
    card_8,
    card_9,
    card_10,
    card_A,
};

static inline cards parse_card( const string& card ) {
    if ( card == "2" ) return card_2;
    else if ( card == "3" ) return card_3;
    else if ( card == "4" ) return card_4;
    else if ( card == "5" ) return card_5;
    else if ( card == "6" ) return card_6;
    else if ( card == "7" ) return card_7;
    else if ( card == "8" ) return card_8;
    else if ( card == "9" ) return card_9;
    else if ( card == "D" || card == "J" || card == "Q" || card == "K" ) return card_10;
    else if ( card == "d" || card == "j" || card == "q" || card == "k" ) return card_10;
    else if ( card == "A" ) return card_A;
    else if ( card == "a" ) return card_A;

    else return error_card;
}

static inline void cls() {
#if defined(_WIN32) || defined(_WIN64)
    system( "cls" );
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__unix__) || defined(__sun)
    system( "clear" );
#endif
}

void simulation( uint_fast64_t amount ) {
    cls();
    random_device dev;
    uniform_int_distribution<int> random_card( card_2, card_A );

    cout << " <----------------------------------------------------------> \n\n";
    string input;
    cards player_card;
    cards dealer_card;

    bool dealer_ace( false ), player_ace( false );
    uint_fast8_t player_score( 0 ), low_player_score( 21 );
    uint_fast8_t dealer_score( 0 ), low_dealer_score( 21 );
    uint_fast8_t rc;

    uint_fast64_t wins( 0 );
    uint_fast64_t daws( 0 );
    uint_fast64_t dws[5] = { 0,0,0,0,0 };

    // inputs
    cout << " 2 3 4 5 6 7 8 9 D J Q K A \n";
    for ( uint_fast8_t i( 0 ); i < 2; ++i ) {
        cout << " Player's card >> "; cin >> input;

        if ( input == "x" || input == "X" )
            break;

        player_card = parse_card( input );

        // ace handling
        if ( player_card == card_A ) {
            if ( player_ace )
                player_score += 1;
            else
                player_score += 11;

            player_ace = true;
        }

        // not ace handling
        else
            player_score += player_card;

        // lower ace score
        low_player_score = player_score - 10;
    }
    cout << "\n Dealer's card >> "; cin >> input;
    dealer_card = parse_card( input );

    if ( player_score > 21 ) {
        // if overflowed use the ace
        if ( player_ace )
            player_score = low_player_score;

        // tactic didn't work because of player's overflow
        else
            return;
    }

    // the lower score can also be overflowed
    if ( player_score > 21 )
        return;

    do {
        // THE SIMULATION
        for ( uint_fast64_t i( 0 ); i < amount; ++i ) {
            // setup
            dealer_score = dealer_card;

            // dealer starting ace test
            dealer_ace = (dealer_card == card_A);
            if ( dealer_ace )
                low_dealer_score = 1;
            else
                low_dealer_score = 21;

            // simulating dealer's hand
            while ( dealer_score < 17 || (dealer_ace && (low_dealer_score < 17)) ) {
                rc = random_card( dev );

                // handling aces
                if ( rc == card_A ) {
                    if ( dealer_ace )
                        dealer_score += 1;
                    else
                        dealer_score += 11;

                    dealer_ace = true;
                } else dealer_score += rc;

                if ( dealer_ace )
                    low_dealer_score = dealer_score - 10;
            }

            if ( dealer_score > 21 ) {
                // if overflowed use the ace
                if ( dealer_ace )
                    dealer_score = low_dealer_score;

                // tactic worked because of dealer's overflow
                else {
                    ++wins;
                    continue;
                }
            }

            // the lower score can also be overflowed
            if ( dealer_score > 21 ) {
                ++wins;
                continue;
            }

            // tactic worked because of winning score
            if ( player_score >= dealer_score ) {
                ++wins;
                continue;
            }

            // womp womp tactic bad, let's save statistics
            daws += dealer_score;
            ++dws[dealer_score - 17];
        }

        // winning percentage
        long double percentage( (long double)(wins) / (long double)(amount) );

        // the results
        cout << "\n <-----> \n";
        cout << " The calculated decision >> \n";

        if ( wins >= amount / 2 ) {
            cout << " - " << percentage * 100 << " %   > STAND <\n";
            cout << " - " << (1 - percentage) * 100 << " %     hit\n";
        } else {
            cout << " - " << percentage * 100 << " %     stand\n";
            cout << " - " << (1 - percentage) * 100 << " %   > HIT <\n";
        }
        cout << " <-----> \n\n";

        cout << "\n <----------------------------------------------------------> \n\n";

        if ( wins >= amount / 2 )
            break;

        // another inputs
        cout << " 2 3 4 5 6 7 8 9 D J Q K A ( to start new simulation X ) \n\n";
        cout << " Player's card >> "; cin >> input;

        if ( input == "x" || input == "X" )
            break;

        player_card = parse_card( input );

        // ace handling
        if ( player_card == card_A ) {
            if ( player_ace )
                player_score += 1;
            else
                player_score += 11;

            player_ace = true;
        }

        // not ace handling
        else
            player_score += player_card;

        // lower ace score
        low_player_score = player_score - 10;

        if ( player_score > 21 ) {
            // if overflowed use the ace
            if ( player_ace )
                player_score = low_player_score;

            // tactic didn't work because of player's overflow
            else
                break;
        }

        // the lower score can also be overflowed
        if ( player_score > 21 )
            break;
    } while ( true );

    cout << " Press any key to start new simulation.\n";
    cin.ignore();
    cin.get();
}

//void rainbow_table() {
//    ifstream file( "bjrt.txt" );
//    vector<string*> tokens;
//    string search;
//    bool searching( false );
//
//    if ( file ) {
//        string line;
//
//        // dividing the file into tokens
//        while ( getline( file, line ) ) {
//            string token;
//            istringstream iss( line );
//            while ( iss >> token )
//                tokens.push_back( new string( token ) );
//        }
//
//        while ( 1 ) {
//            cls();
//            cout << "\n\n";
//            cout << "    |                                |\n";
//            cout << "    |    Blackjack Rainbow Tables    |\n";
//            cout << "    |                                |\n\n\n";
//            cout << " You can search by typing player score.\n";
//            cout << " Type X to reset searching. \n\n\n";
//
//            cout << "    |   Player score   |   Dealer score   |   hit %   |   stand %   |\n\n";
//
//            for ( uint_fast64_t i( 0 ); i < tokens.size(); i += 4 ) {
//                if ( searching )
//                    if ( *tokens[i] != search )
//                        continue;
//
//                cout << "    |";
//            
//                // player score
//                for ( uint_fast8_t n( 0 ); n < (18 - tokens[i]->size()) / 2; ++n )
//                    cout << " ";
//                cout << *tokens[i];
//                for ( uint_fast8_t n( 0 ); n < (18 - tokens[i]->size()) / 2; ++n )
//                    cout << " ";
//                if ( (18 - tokens[i]->size()) % 2 ) cout << " ";
//                cout << "|";
//
//                // dealer score
//                for ( uint_fast8_t n( 0 ); n < (18 - tokens[i + 1]->size()) / 2; ++n )
//                    cout << " ";
//                cout << *tokens[i + 1];
//                for ( uint_fast8_t n( 0 ); n < (18 - tokens[i + 1]->size()) / 2; ++n )
//                    cout << " ";
//                if ( (18 - tokens[i + 1]->size()) % 2 ) cout << " ";
//                cout << "|";
//
//                // HIT chance
//                for ( uint_fast8_t n( 0 ); n < (12 - tokens[i + 2]->size()) / 2; ++n )
//                    cout << " ";
//                cout << *tokens[i + 2];
//                for ( uint_fast8_t n( 0 ); n < (12 - tokens[i + 2]->size()) / 2; ++n )
//                    cout << " ";
//                if ( (12 - tokens[i + 2]->size()) % 2 ) cout << " ";
//                cout << "|";
//
//                // player score
//                for ( uint_fast8_t n( 0 ); n < (12 - tokens[i + 3]->size()) / 2; ++n )
//                    cout << " ";
//                cout << *tokens[i + 3];
//                for ( uint_fast8_t n( 0 ); n < (12 - tokens[i + 3]->size()) / 2; ++n )
//                    cout << " ";
//                if ( (12 - tokens[i + 3]->size()) % 2 ) cout << " ";
//                cout << "|\n";
//            }
//
//            cout << "\n\n >> "; cin >> search;
//
//            if ( search != "X" && search != "x" ) searching = true;
//            else searching = false;
//        }
//    } else {
//        cout << " Unable to find blackjack rainbow table file ( 'bjrt.txt' ).\n";
//        cout << " Visit 'https://github.com/krisluczka' and download the newest version.\n";
//    }
//
//    file.close();
//    cin.ignore();
//    cin.get();
//
//    for ( string* token : tokens )
//        delete token;
//}

int main() {
    string input;

    cout << " /--------------------------------------\\ \n";
    cout << " |                                      |\n";
    cout << " |   Blackjack probabilistic            |\n";
    cout << " |               calculator version 1   |\n";
    cout << " |                                      |\n";
    cout << " |                by Krzysztof Luczka   |\n";
    cout << " |                                      |\n";
    cout << " \\--------------------------------------/ \n\n";

    cout << "     Type every card that was in the game \n"
        << " (first player's, then the dealer's). \n"
        << " The calculator will then simulate through \n"
        << " milions of games to reveal the actual\n"
        << " chances of winning.\n\n\n";

    cout << "    1. Dynamic mode\n";
    cout << "         Type player cards on the fly\n";
    cout << "         without additional statistics.\n";
    /*cout << "    2. Rainbow tables\n";
    cout << "         Table with precalculated stats.\n\n";*/
    cout << " >> "; cin >> input;


    simulation( 1000000 );
}