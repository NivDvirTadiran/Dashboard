export interface StoryInput {
  id: string;
  title: string;
  state: string;
  icon:  string;
  type:  'text' | 'password' | 'tel' | 'email' ;
  placeholder: string;
  hide: boolean;

}
