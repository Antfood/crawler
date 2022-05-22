#include "../include/user_input.hpp"

struct UserInput::Private
{
  static Component create_event (Changeset &changeset, const std::string &path, ScreenInteractive &screen, const Component &renderer)
  {
    return CatchEvent (renderer, [&] (Event const &event)
    {
      if (event == Event::Character ('\n'))
        {
          screen.ExitLoopClosure () ();
          return true;
        }

        if(event == Event::Special({6})) // CTRL +  F
        {
          std::string cmd = "open \"";
          cmd += path;
          cmd += "\"";
          std::system(cmd.c_str());
         return true;
        }

      if(event == Event::Special({14})) //  CTRL + N
        {
          changeset.m_skipped = true;
          screen.ExitLoopClosure () ();
          return true;
        }

        if(event == Event::Special({5})) //  CTRL + E
        {
          changeset.m_quit = true;
          screen.ExitLoopClosure () ();
          return true;
        };
      return false;
    });
  }
   static Color::Palette16 which_color(Changeset &changeset , error_type err)
   {
    return changeset.has_error (err) ? Color::Red : Color::White;
   }

   static Element render_more_errors(Changeset &changeset)
   {
     if (changeset.m_errors.size() > 1)
       return text("Next errors -> " + changeset.other_errors()) | color(Color::Red);
     else
       return text("");
   };
};

UserInput::UserInput (Changeset &changeset, const std::string &path)
{

  std::cout << "\n\n";
  changeset.fill_empty_fields ();

  Component client_input                = Input(&changeset.m_fields[client], "Missing Client Name");
  Component project_input               = Input(&changeset.m_fields[project], "Missing Project Name");
  Component recording_name_input        = Input(&changeset.m_fields[name], "Missing Recording Name");
  Component recording_descriptor_input  = Input(&changeset.m_fields[descriptor], "Missing Descriptor Name");
  Component date_input                  = Input(&changeset.m_fields[date], "Missing Date");
  Component key_input                   = Input(&changeset.m_fields[key], "Missing Key");
  Component bpm_input                   = Input(&changeset.m_fields[bpm], "Missing BPM");
  Component ts_input                    = Input(&changeset.m_fields[ts], "Missing time Signature");
  Component composer_input              = Input(&changeset.m_fields[composer], "Missing Composer Initials");
  Component talent_input                = Input(&changeset.m_fields[external_talent], "Missing External Talent or NXT");

  Component project_container = Container::Vertical({
                                                     client_input,
                                                     project_input,
                                                     recording_name_input,
                                                     recording_descriptor_input,
                                                     date_input,
                                                     key_input,
                                                     bpm_input,
                                                     ts_input,
                                                     composer_input,
                                                     talent_input
                                                     });

  Component renderer = Renderer(project_container, [&] {
    return  vbox({
               bold(text(" " + changeset.build_path())) | color(Color::White),
               text(" " + changeset.first_error()) | color(Color::Red),
               Private::render_more_errors (changeset),
               separator(),
               text(" 'CTRL + F' to reveal in Finder.") | color(Color::Blue),
               text(" 'CTRL + E' to exit.") | color(Color::Blue),
               text(" 'CTRL + N' to skip.") | color(Color::Blue),
               separator(),
               hbox(text(" Client:"), client_input->Render()  | color(Private::which_color (changeset, bad_client))),
               hbox(text(" Project: "), project_input->Render() | color(Private::which_color (changeset, bad_project)) ),
               hbox(text(" Recording: "), recording_name_input->Render() | color(Private::which_color (changeset, bad_name))),
               hbox(text(" Descriptor: "), recording_descriptor_input->Render() | color(Private::which_color (changeset, bad_descriptor))),
               hbox(text(" Date: "), date_input->Render() | color(Private::which_color (changeset, bad_date))),
               hbox(text(" Key: "), key_input->Render() | color(Private::which_color (changeset, bad_key))),
               hbox(text(" BPM: "), bpm_input->Render() | color(Private::which_color (changeset, bad_bpm))),
               hbox(text(" Time Signature: "), ts_input->Render() | color(Private::which_color (changeset, bad_ts))),
               hbox(text(" Composer: "), composer_input->Render() | color(Private::which_color (changeset, bad_composer))),
               hbox(text(" Talent: "), talent_input->Render() | color(Private::which_color (changeset, bad_external_talent))),
           }) | border;
  });

  auto screen            = ScreenInteractive::TerminalOutput();
  auto event                   = Private::create_event(changeset, path, screen, renderer);
  screen.Loop(event);
};