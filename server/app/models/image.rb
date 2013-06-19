class Image < ActiveRecord::Base
  validates :kind_id, :presence => true
  validates :location, :presence => true, :uniqueness => true

  attr_accessible :kind_id, :like, :location, :name, :unlike
  belongs_to :kind
end
