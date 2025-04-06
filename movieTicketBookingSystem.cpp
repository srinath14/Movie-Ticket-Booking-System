#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>>
using namespace std;
// ------------------- ENUMS -------------------
enum MovieStatus { Not_BOOKED = 0, PENDING, BOOKED };

// ------------------- DTO -------------------
class MovieMetaData {
public:
    string cityName;
    string movieName;
    string startTime;
    string language;
    string date;

    MovieMetaData(string city, string name, string time, string lang, string dt)
        : cityName(city), movieName(name), startTime(time), language(lang), date(dt) {}
};

// ------------------- MODELS -------------------
class Movie {
    int movieId;
    string name;
    string duration;
    string language;

public:
    Movie(int id, string nm, string dur, string lang)
        : movieId(id), name(nm), duration(dur), language(lang) {}

    string getName() const { return name; }
    string getLanguage() const { return language; }
    int getId() const { return movieId; }
};

class Seat {
public:
    char row;
    int number;
    int seatId;
    MovieStatus status;

    Seat(char r, int n, int id)
        : row(r), number(n), seatId(id), status(Not_BOOKED) {}

    bool isAvailable() const { return status == Not_BOOKED; }
    void book() { status = BOOKED; }
};

class Show {
public:
    Movie movie;
    string startTime;
    vector<Seat> seats;

    Show(Movie m, string time) : movie(m), startTime(time) {
        // Create sample seats
        for (int i = 0; i < 10; ++i)
            seats.emplace_back('A', i + 1, i + 1);
    }

    Movie getMovie() const { return movie; }
    string getStartTime() const { return startTime; }

    vector<Seat*> getAvailableSeats() {
        vector<Seat*> available;
        for (auto &seat : seats)
            if (seat.isAvailable())
                available.push_back(&seat);
        return available;
    }
};

class Ticket {
public:
    int ticketId;
    string movieName;
    string showTime;
    vector<Seat> bookedSeats;

    Ticket(int id, const string &mName, const string &time, const vector<Seat> &seats)
        : ticketId(id), movieName(mName), showTime(time), bookedSeats(seats) {}

    void printTicket() const {
        cout << "Ticket ID: " << ticketId << ", Movie: " << movieName << ", Time: " << showTime << ", Seats: ";
        for (const auto &seat : bookedSeats)
            cout << seat.row << seat.number << " ";
        cout << endl;
    }
};

class User {
public:
    string name, email;
    vector<Ticket> tickets;

    User(string n, string e) : name(n), email(e) {}

    void addTicket(const Ticket &ticket) { tickets.push_back(ticket); }

    void showTickets() {
        for (const auto &ticket : tickets)
            ticket.printTicket();
    }
};

// ------------------- THEATRE -------------------
class Theatre {
    string name;
    unordered_map<string, vector<Show>> calendar;

public:
    Theatre(string nm) : name(nm) {}

    void addShow(const string &date, const Show &show) {
        calendar[date].push_back(show);
    }

    vector<Show*> searchShows(const MovieMetaData &meta) {
        vector<Show*> results;
        auto &shows = calendar[meta.date];
        for (auto &show : shows) {
            if (show.getMovie().getName() == meta.movieName &&
                show.getStartTime() == meta.startTime &&
                show.getMovie().getLanguage() == meta.language)
                results.push_back(&show);
        }
        return results;
    }
};

// ------------------- THEATRE -------------------
class Theatre {
    string name;
    unordered_map<string, vector<Show>> calendar;

public:
    Theatre(string nm) : name(nm) {}

    void addShow(const string &date, const Show &show) {
        calendar[date].push_back(show);
    }

    vector<Show*> searchShows(const MovieMetaData &meta) {
        vector<Show*> results;
        auto &shows = calendar[meta.date];
        for (auto &show : shows) {
            if (show.getMovie().getName() == meta.movieName &&
                show.getStartTime() == meta.startTime &&
                show.getMovie().getLanguage() == meta.language)
                results.push_back(&show);
        }
        return results;
    }
};
class TicketManager {
    int ticketCounter = 1000;
    vector<Ticket> issuedTickets;

public:
    Ticket createTicket(const string &movieName, const string &time, const vector<Seat> &seats) {
        Ticket ticket(ticketCounter++, movieName, time, seats);
        issuedTickets.push_back(ticket);
        return ticket;
    }
};
//movie Booking manager
// ------------------- BOOKING MANAGER -------------------
class MovieBookingManager {
    unordered_map<string, vector<Theatre>> cityTheatres;
    TicketManager ticketManager;

public:
    void registerTheatre(const string &city, const Theatre &theatre) {
        cityTheatres[city].push_back(theatre);
    }

    bool bookMovie(const MovieMetaData &meta, User &user) {
        auto &theatres = cityTheatres[meta.cityName];
        for (auto &theatre : theatres) {
            auto shows = theatre.searchShows(meta);
            for (auto *show : shows) {
                auto available = show->getAvailableSeats();
                if (!available.empty()) {
                    // Book the first seat
                    available[0]->book();
                    vector<Seat> booked = { *available[0] };
                    Ticket ticket = ticketManager.createTicket(meta.movieName, meta.startTime, booked);
                    user.addTicket(ticket);
                    cout << "Booking successful!\n";
                    return true;
                }
            }
        }
        cout << "No available seats found.\n";
        return false;
    }
};

// ------------------- SYSTEM ENTRY -------------------
class BookingSystem {
    MovieBookingManager manager;

public:
    BookingSystem() {
        // Sample setup
        Movie movie1(1, "Inception", "2h 30m", "English");
        Show show1(movie1, "18:00");
        Theatre theatre1("PVR");
        theatre1.addShow("2025-04-10", show1);
        manager.registerTheatre("Hyderabad", theatre1);
    }

    void startBooking() {
        User user("Srinath", "sri@example.com");
        MovieMetaData meta("Hyderabad", "Inception", "18:00", "English", "2025-04-10");
        manager.bookMovie(meta, user);
        user.showTickets();
    }
};

// ------------------- MAIN -------------------
int main() {
    BookingSystem system;
    system.startBooking();
    return 0;
}