class ChatsController < ApplicationController
  def index
    @chat_cookies =  cookies[:chat_name]
    @chats = Chat.all.limit(20).order("created_at DESC")
  end
  def create
    @chat = Chat.new
    @chat.name = params[:chat][:name]
    @chat.mesg = params[:chat][:mesg]
    @chat.save
    cookies[:chat_name] = params[:chat][:name]
    redirect_to '/chats/index'
  end
end
