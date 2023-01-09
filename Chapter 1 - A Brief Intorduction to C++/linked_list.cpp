int num_hamlet(const std::forward_list<std::string> &books) {
  return std::count(books.begin(), books.end(), "Hamlet");
}
