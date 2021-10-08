from random import randint


cache = dict()


def generate_line(size):
    cards = [0, 0]
    line = []

    for i in range(2, 15):
        cards.append(4)

    while len(line) < size:
        card = randint(2, 14)
        if cards[card] > 0:
            cards[card] -= 1
            line.append(card)

    return line


def greedy(line):
    select = 0 if line[0] > line[-1] else -1
    value = line[select]
    del line[select]
    return value


def greedy_smart(line):
    if len(line) < 4:
        return greedy(line)

    max_lose_left = 1 if line[1] > line[-1] else -1
    max_lose_right = 0 if line[0] > line[-2] else -2

    left_revenue = line[0] - line[max_lose_left]
    right_revenue = line[-1] - line[max_lose_right]

    select = 0 if left_revenue > right_revenue else -1
    value = line[select]
    del line[select]
    return value


def dynamic(line, is_turn):
    key = ' '.join([str(i) for i in line])

    if key in cache:
        return cache[key]

    if len(line) <= 0:
        return 0

    if not is_turn:
        if line[0] > line[-1]:
            return dynamic(line[1:], not is_turn)
        else:
            return dynamic(line[:-1], not is_turn)

    if len(line) < 3:
        select = 0 if line[0] > line[-1] else -1
        value = line[select]
        return value

    left_sum = dynamic(line[1:], not is_turn) + line[0]
    right_sum = dynamic(line[:-1], not is_turn) + line[-1]

    cache[key] = max(left_sum, right_sum)
    return max(left_sum, right_sum)


def dynamic_selection(line):
    left_sum = dynamic(line[1:], False) + line[0]
    right_sum = dynamic(line[:-1], False) + line[-1]
    select = 0 if left_sum > right_sum else -1
    value = line[select]
    del line[select]
    return value


if __name__ == '__main__':
    player_one_count = 0
    player_two_count = 0
    card_line = generate_line(50)
    # card_line = [4, 7, 3, 13, 6, 6, 13, 13, 2, 2]
    # card_line = [6, 9, 12, 6, 5, 10, 3, 4, 11, 14]
    # card_line = [4, 14, 2, 7, 5, 12]
    print(card_line)

    while len(card_line):
        selection = greedy(card_line)
        player_one_count += selection
        print(f"player one selected: {selection}")
        print(card_line)

        if len(card_line):
            selection = dynamic_selection(card_line)
            player_two_count += selection
            print(f"player two selected: {selection}")
            print(card_line)

    if player_one_count > player_two_count:
        print(f"player_one win with score: {player_one_count}")
        print(f"player_two score: {player_two_count}")
    elif player_one_count == player_two_count:
        print(f"player_two score: {player_two_count}")
        print(f"player_one score: {player_one_count}")
    else:
        print(f"player_two win with score: {player_two_count}")
        print(f"player_one score: {player_one_count}")
