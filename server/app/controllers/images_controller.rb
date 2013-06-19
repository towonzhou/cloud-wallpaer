class ImagesController < ApplicationController
  # GET /images
  # GET /images.json
  def index
    @images = Image.all

    respond_to do |format|
      #format.html # index.html.erb
      format.json { render json: @images }
    end
  end

  def kinds_list
    @kind = Kind.all

    respond_to do |format|
      format.json { render json: @kind }
    end
  end

  def images_list
    @kind = Kind.find_by_name(params[:kind])
    @images = Image.find_all_by_kind_id(@kind.id)

    respond_to do |format|
      format.json { render json: @images }
    end
  end

  def evaluate
    @image = Image.find(params[:id])
    @evaluate = params[:evaluate]

    if @evaluate == 'like'
      @image.like += 1
    elsif @evaluate == 'unlike'
      @image.unlike += 1
    end

    respond_to do |format|
      if @image.save
        #format.html { redirect_to @image, notice: 'evaluate was successfully.' }
        format.json { render json: @image, status: :created, location: @image }
      else
        #format.html { render action: "index" }
        format.json { render json: @image.errors, status: :unprocessable_entity }
      end
    end
  end

  # GET /images/1
  # GET /images/1.json
  def show
    @image = Image.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @image }
    end
  end


  # GET /images/new
  # GET /images/new.json
  def new
    @image = Image.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @image }
    end
  end

  # GET /images/1/edit
  def edit
    @image = Image.find(params[:id])
  end

  # POST /images
  # POST /images.json
  def create
    @image = Image.new(params[:image])

    respond_to do |format|
      if @image.save
        format.html { redirect_to @image, notice: 'Image was successfully created.' }
        format.json { render json: @image, status: :created, location: @image }
      else
        format.html { render action: "new" }
        format.json { render json: @image.errors, status: :unprocessable_entity }
      end
    end
  end

  # PUT /images/1
  # PUT /images/1.json
  def update
    @image = Image.find(params[:id])

    respond_to do |format|
      if @image.update_attributes(params[:image])
        format.html { redirect_to @image, notice: 'Image was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: "edit" }
        format.json { render json: @image.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /images/1
  # DELETE /images/1.json
  def destroy
    @image = Image.find(params[:id])
    @image.destroy

    respond_to do |format|
      format.html { redirect_to images_url }
      format.json { head :no_content }
    end
  end
end
