require 'test/unit'
require 'rubygems'
require 'bundler'
Bundler.setup
require 'shoulda'
require 'mocha'
require 'tiny_tds'

module TinyTds 
  class TestCase < Test::Unit::TestCase
    
    def test_base_tiny_tds_case ; assert(true) ; end
    
    
    protected
    
    def connection_options(options={})
      { :host          => ENV['TINYTDS_UNIT_HOST'] || 'localhost',
        :username      => 'tinytds',
        :password      => '',
        :database      => 'tinytds_test',
        :appname       => 'TinyTds Dev',
        :login_timeout => 5,
        :timeout       => 5
      }.merge(options)
    end
    
    def assert_raise_tinytds_error(action)
      error_raised = false
      begin
        action.call
      rescue TinyTds::Error => e
        error_raised = true
      end
      assert error_raised, 'expected a TinyTds::Error but none happened'
      yield e
    end
    
    def inspect_tinytds_exception
      begin
        yield
      rescue TinyTds::Error => e
        props = { :source => e.source, :message => e.message, :severity => e.severity, 
                  :db_error_number => e.db_error_number, :os_error_number => e.os_error_number }
        raise "TinyTds::Error - #{props.inspect}"
      end
    end
    
  end
end
