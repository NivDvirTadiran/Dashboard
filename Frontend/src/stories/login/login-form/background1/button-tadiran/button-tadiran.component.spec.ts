import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ButtonTadiranComponent } from './button-tadiran.component';

describe('LanguageIconComponent', () => {
  let component: ButtonTadiranComponent;
  let fixture: ComponentFixture<ButtonTadiranComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ButtonTadiranComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ButtonTadiranComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
