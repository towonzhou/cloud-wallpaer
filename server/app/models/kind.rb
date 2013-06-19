class Kind < ActiveRecord::Base
  validates :name, :presence => true, :uniqueness => true

  attr_accessible :alias, :name
  has_many :images
end
