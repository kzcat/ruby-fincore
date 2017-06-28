# coding: utf-8

$:.unshift File.join(File.dirname(__FILE__), 'lib')

require 'fincore/version'

Gem::Specification.new do |spec|
  spec.name          = 'fincore'
  spec.version       = Fincore::VERSION
  spec.authors       = ['kazuto.okada@gmail.com']
  spec.email         = ['kazuto.okada@gmail.com']
  spec.summary       = %q{counts pages of file contents in core }
  spec.description   = %q{counts pages of file contents being resident in memory(in core), and reports the numbers.}
  spec.homepage      = 'https://github.com/kzcat/ruby-fincore'
  spec.license       = 'MIT'
  spec.extensions    = %w[ext/fincore/extconf.rb]
  spec.files         = %w(ext/fincore/extconf.rb ext/fincore/fincore.c Rakefile fincore.gemspec Gemfile lib/fincore.rb lib/fincore/version.rb README.md)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]
end
