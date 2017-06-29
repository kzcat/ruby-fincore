# ruby-fincore 
Ruby File class extension to determine pages of file contents being resident in memory(in core).

### Installation

To install:

```sh
$ gem install fincore
```

### Usage
```ruby
require 'fincore'

File.open('some/file') do |f|
  f.cached_pages # return number of pages that are cached
  f.total_pages  # return number of pages at total
end
```
work with `IO#advise`
```ruby
require 'fincore'

f = File.open('some/file') do |f|
  f.advise(:willneed) # tell the kernel file contents will be accessed.
  f.cached_pages
  => 620129
  f.advise(:dontneed) # tell the kernel no more access.
  f.cached_pages
  => 0
end
```

### Instance methods for File class
* `cached_pages -> Integer`
* `total_pages -> Integer`

### Requirements
Platform must support `mincore(2)` system call.
