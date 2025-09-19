def split_input(input_str):
    return [token.strip() for token in input_str.split(",") if token.strip()]


def main():
    # Input retrieved documents (Answer set A)
    retrieved_input = input("Enter retrieved documents (comma-separated, in rank order): ")
    answer_set = split_input(retrieved_input)

    # Input relevant documents (Set R)
    relevant_input = input("Enter relevant documents (comma-separated): ")
    relevant_set = set(split_input(relevant_input))

    total_relevant = len(relevant_set)   # |R|
    retrieved_relevant_count = 0         # |Ra|

    # Print table header
    print("\n{:<60}{:<20}{:<20}{:<15}{:<15}".format(
        "Documents Retrieved So Far",
        "Relevant∩Retrieved (|Ra|)",
        "Retrieved Count (|A|)",
        "Precision (%)",
        "Recall (%)"
    ))
    print("-" * 130)

    # Iterate over retrieved docs
    for i, doc in enumerate(answer_set):
        if doc in relevant_set:
            retrieved_relevant_count += 1

        precision = (retrieved_relevant_count / (i + 1)) * 100
        recall = (retrieved_relevant_count / total_relevant) * 100 if total_relevant > 0 else 0

        # Build retrieved docs so far string
        docs_so_far = ", ".join(answer_set[:i + 1])

        # Print row
        print("{:<60}{:<20}{:<20}{:<15.2f}{:<15.2f}".format(
            docs_so_far,
            retrieved_relevant_count,
            i + 1,
            precision,
            recall
        ))


if __name__ == "__main__":
    main()
