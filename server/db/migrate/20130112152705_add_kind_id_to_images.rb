class AddKindIdToImages < ActiveRecord::Migration
  def change
    add_column :images, :kind_id, :integer
  end
end
